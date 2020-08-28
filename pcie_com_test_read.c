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
#include <csignal>
#include <memory>
#include "pcie_com.h"

#define DEVNAME "/dev/pciecom"

#define PAGE_NUM 1

using namespace std;

int g_filefd;
int ionfd;  // /dev/ion的fd
int g_mapfd;
int heap_count;	
std::vector<struct ion_heap_data> ion_heaps;  // 内核所以 heaps信息
unsigned char* g_ptr;

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

void Release() {
    int ret = munmap(g_ptr, getpagesize() * PAGE_NUM);
    if(ret!=0) {
        cout<<"ERROR: munmap failed"<<endl;
    }

    ret = close(g_mapfd);
    if(ret!=0) {
        cout<<"ERROR: close failed"<<endl;
    }    

    ret = ion_close(ionfd);
    if(ret!=0) {
        cout<<"ERROR: close failed"<<endl;
    }
    close(g_filefd);
}

void OnShutdown(int sig) {
    (void)sig;
    Release();
}

int main()
{
    std::signal(SIGINT, OnShutdown);
    int fd;
    fd = open(DEVNAME, O_RDWR);
    if(fd == -1)
    {
        printf("file %s is opening......failure!", DEVNAME);
    }
    else
    {
        printf("file %s is opening......successfully!\nits fd is %d\n", DEVNAME, fd);
    }
    g_filefd = fd;

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

        ret = ion_alloc_fd(ionfd, getpagesize() * PAGE_NUM, 0, (1 << heap.heap_id), 0, &map_fd); // 对指定的heap分配2页内存
        if(ret!=0 || map_fd <= 0){
            cout<<"ERROR: ion_alloc_fd failed"<<endl;
            return -ALLOC_ERR;
        }
        g_mapfd = map_fd;

        unsigned char* ptr;
        ptr = (unsigned char*)mmap(NULL, getpagesize() * PAGE_NUM, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0); 
        if(ptr == MAP_FAILED){
            cout<<"ERROR: mmap failed"<<endl;
            return -MMAP_ERR;
        }
        g_ptr = ptr;

        memset(ptr, 0x0, getpagesize() * PAGE_NUM);

        // ret =munmap(ptr, getpagesize() * PAGE_NUM);

        // if(ret!=0) {
        //     cout<<"ERROR: munmap failed"<<endl;
        //     return -MUNMAP_ERR;
        // }

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

        // ret = close(map_fd);  // close 函数将释放map_fd对应的内存
        // if(ret!=0) {
        //     cout<<"ERROR: close failed"<<endl;
        //     return -CLOSE_MAP_FD_ERR;
        // }    
    }
    // ret = ion_close(ionfd);
    // if(ret!=0) {
    //     cout<<"ERROR: close failed"<<endl;
    //     return -CLOSE_ION_FD_ERR;
    // }
    // close(fd);

    std::shared_ptr<unsigned char []> buffer = std::make_shared<unsigned char []>(getpagesize() * PAGE_NUM);
    ret = read(g_filefd, buffer.get(), getpagesize() * PAGE_NUM);
    if (ret < 0) {
        cout << "###read data failed!" << std::endl;
    } else {
        cout << "###read data: " << buffer.get() << std::endl;
    }

    return 0;
}