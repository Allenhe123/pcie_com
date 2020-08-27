#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>			// for	getpagesize
#include <ion/ion.h> 		// libion header
#include <linux/ion_4.19.h> // struct ion_heap_data
#include <memory.h>
#include <vector>
#include <iostream> 
#include "pcie_com.h"

#define DEVNAME "/dev/pciecom"

using namespace std;

int ionfd;  // /dev/ion的fd
int heap_count;	
std::vector<struct ion_heap_data> ion_heaps;  // 内核所以 heaps信息

enum e_ErrorCode{
    OKAY,
    OPEN_ERR,
    QUREY_HEAD_CNT_ERR,
    QUREY_HEAD_INFO_ERR,
    ALLOC_ERR,
    MMAP_ERR,
    MUNMAP_ERR,
    MEM_VALUE_ERR,
    CLOSE_MAP_FD_ERR,
    CLOSE_ION_FD_ERR,
};

int main()
{
      char alpha[27];
    int fd,i;
    memset(alpha, 0, 27);

    for(i = 0; i < 26; i++)
        alpha[i] = 'A' + i;

    fd = open(DEVNAME, O_RDWR);
    if(fd == -1)
    {
        printf("file %s is opening......failure!", DEVNAME);
    }
    else
    {
        printf("file %s is opening......successfully!\nits fd is %d\n", DEVNAME, fd);
    }


    	ionfd = ion_open(); // 打开 /dev/ion
    if(ionfd<=0) {
        cout<<"ion_open failed"<<endl;
        return -OPEN_ERR;
    }    
    
    int ret = ion_query_heap_cnt(ionfd, &heap_count);
    if(ret!=0){
        cout<<"ion_query_heap_cnt failed"<<endl;
        return -QUREY_HEAD_CNT_ERR;
    }
    ion_heaps.resize(heap_count, {});
    ret = ion_query_get_heaps(ionfd, heap_count, ion_heaps.data());
    if(ret!=0){
        cout<<"ion_query_get_heaps failed"<<endl;
        return -QUREY_HEAD_INFO_ERR;
    }

    // 每个heap分配一定的空间
    for (const auto& heap : ion_heaps) {
        cout<< "heap:" << heap.name << ":" << heap.type << ":" << heap.heap_id<<endl;
        int map_fd = -1;
        
        if (heap.type != 4) continue;

        ret = ion_alloc_fd(ionfd, getpagesize() * 2, 0, (1 << heap.heap_id), 0, &map_fd); // 对指定的heap分配2页内存
        if(ret!=0 || map_fd <= 0){
            cout<<"ERROR: ion_alloc_fd failed"<<endl;
            return -ALLOC_ERR;
        }

        unsigned char* ptr;
        ptr = (unsigned char*)mmap(NULL, getpagesize() * 2, PROT_READ | PROT_WRITE, MAP_SHARED,
                                   map_fd, 0); // map 2页内存到用户态
        if(ptr == MAP_FAILED){
            cout<<"ERROR: mmap failed"<<endl;
            return -MMAP_ERR;
        }

        memset(ptr, 0xaa, getpagesize() * 2);// 第1页内存设置为0

        ret =munmap(ptr, getpagesize() * 2);
        if(ret!=0) {
            cout<<"ERROR: munmap failed"<<endl;
            return -MUNMAP_ERR;
        }

        ret = ioctl(fd, ION_IOC_TEST_SET_FD, map_fd);
        if (ret != 0) {
            printf("ioctl ERROR\n");
        }

        struct ion_test_rw_data data;
        data.write = 0;
        data.offset = 0;

        ret = ioctl(fd, ION_IOC_TEST_DMA_MAPPING, &data);
        if (ret != 0) {
            printf("ioctl ERROR\n");
        }

        ret = close(map_fd);  // close 函数将释放map_fd对应的内存
        if(ret!=0) {
            cout<<"ERROR: close failed"<<endl;
            return -CLOSE_MAP_FD_ERR;
        }    
    }
    ret = ion_close(ionfd);
    if(ret!=0) {
        cout<<"ERROR: close failed"<<endl;
        return -CLOSE_ION_FD_ERR;
    }  
    
    cout<<"all heaps test OK"<<endl;

    close(fd);

    return 0;


    // char alpha[27];
    // int fd,i;
    // memset(alpha, 0, 27);

    // for(i = 0; i < 26; i++)
    //     alpha[i] = 'A' + i;

    // fd = open(DEVNAME, O_RDWR);
    // if(fd == -1)
    // {
    //     printf("file %s is opening......failure!", DEVNAME);
    // }
    // else
    // {
    //     printf("file %s is opening......successfully!\nits fd is %d\n", DEVNAME, fd);
    // }

    // getchar();

    // printf("write A-Z to kernel......\n");

    // write(fd, alpha, 26);

    // getchar();

    // printf("read datas from kernel.......\n");

    // read(fd, alpha, 26);

    // printf("%s\n", alpha);

    // getchar();

    // close(fd);

    // return 0;
}