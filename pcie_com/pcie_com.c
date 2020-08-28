#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include <linux/dma-buf.h>
#include <linux/dma-direction.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
// #include <linux/kfifo.h>
// #include <linux/moduleparam.h>
// #include <linux/types.h>
// #include <linux/ioport.h>
// #include <linux/io.h>
// #include <linux/delay.h>
// #include "pcie_test.h"

#include "pcie_com.h"

#define VERS_MAJOR 256
#define VERS_MINOR 0
#define VERS_DEV_CNT 1
#define VERS_DEV_NAME "pciecom"

#define u64_to_uptr(x) ((void __user *)(unsigned long)(x))

#define BLOCK_SIZE 4 * 1024

static struct cdev comdev;

struct ion_test_data {
	struct dma_buf *dma_buf;
	struct device *dev;
};

struct ion_test_device {
	struct miscdevice misc;
};

dma_addr_t g_rc_dma_addr;
dma_addr_t g_ep_dma_addr;

static int pciecom_open(struct inode* inode, struct file* filp) {
	printk("file open in pciecom_open......finished!\n");

	struct ion_test_data *data;
	struct miscdevice *miscdev = filp->private_data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->dev = miscdev->parent;
	filp->private_data = data;

	return 0;
}

static int pciecom_release(struct inode* inode, struct file* filp) {
	printk("file release in pciecom_release......finished!\n");
	return 0;
}

static ssize_t pciecom_read(struct file* filp, char __user* buf, size_t count, loff_t* pos) {
	printk("call pciecom_read \n");
	// char tmp[256];
	// int cnt = count;
	// memset(tmp,0,256);
	// int i;
	// for (i=0; i<26; i++)
	// 	tmp[i] = i;
	// if (count > 256) cnt = 256;

	// if (!copy_to_user(buf, tmp, cnt)) {
	// 	return cnt;
	// } else {
	// 	return -1;
	// }

/////// EP read use below code
/*
	extern u32 dm_pcie_dma_read(u32, u32, u64, u64);
	int ret = dm_pcie_dma_read(0, BLOCK_SIZE, g_rc_dma_addr, g_ep_dma_addr);
	if (ret == 0) {
		printk("ep read %d data success!\n", BLOCK_SIZE);
	} else {
		printk("ep read %d data failed!\n", BLOCK_SIZE);
	}
	*/
}

static ssize_t pciecom_write(struct file* filp, const char __user* buf, size_t count, loff_t* pos) {
	printk("call pciecom_write \n");
	// char tmp[256];
	// int cnt = count;
	// memset(tmp,0,256);
	// if (count > 256) cnt = 256;

	// if (!copy_from_user(tmp, buf, cnt)) {
	// 	printk(tmp); printk("###\n");
	// 	return cnt;

	// } else {
	// 	return -1;
	// }

	extern u32 dm_pcie_dma_write(u32, u32, u64, u64);

	// use channel 0, write 0x40 bytes from g_rc_dma_addr to g_ep_dma_addr
	int ret = dm_pcie_dma_write(0, BLOCK_SIZE, g_rc_dma_addr, g_ep_dma_addr); //rc-->ep
	if (ret == 0) {
		printk("rc write %d data success!\n", BLOCK_SIZE);
		return BLOCK_SIZE;
	} else {
		printk("rc write %d data failed!\n", BLOCK_SIZE);
		return -1;
	}
}


static int ion_handle_test_dma(struct device *dev, struct dma_buf *dma_buf,
			       void __user *ptr, size_t offset, size_t size,
			       bool write)
{
	printk("ion_handle_test_dma...write:%d\n", write);
	int ret = 0;
	struct dma_buf_attachment *attach;
	struct sg_table *table;
	pgprot_t pgprot = pgprot_writecombine(PAGE_KERNEL);
	enum dma_data_direction dir = write ? DMA_FROM_DEVICE : DMA_TO_DEVICE;
	struct sg_page_iter sg_iter;
	unsigned long offset_page;

	attach = dma_buf_attach(dma_buf, dev);
	if (IS_ERR(attach))
		return PTR_ERR(attach);
printk("ion_handle_test_dma1...\n");

	table = dma_buf_map_attachment(attach, dir);
	if (IS_ERR(table))
		return PTR_ERR(table);

	struct scatterlist *s;
	int i = 0;

	printk("ion_handle_test_dma2...\n");
	
	// WARN_ON(nents == 0 || sg[0].length == 0);

	for_each_sg(table->sgl, s, table->nents, i) {
		s->dma_address = sg_phys(s);
		g_rc_dma_addr = s->dma_address;
		printk("DMA: 0x%x\n", s->dma_address);

		// if (!check_addr("map_sg", hwdev, s->dma_address, s->length))
		// 	return 0;

		// s->dma_length = s->length;

		// if (attrs & DMA_ATTR_SKIP_CPU_SYNC)
		// 	continue;

		// flush_dcache_range(dma_addr_to_virt(s->dma_address),
		// 		   dma_addr_to_virt(s->dma_address + s->length));
	}

/*
	offset_page = offset >> PAGE_SHIFT;
	offset %= PAGE_SIZE;

	for_each_sg_page(table->sgl, &sg_iter, table->nents, offset_page) {
		struct page *page = sg_page_iter_page(&sg_iter);
		void *vaddr = vmap(&page, 1, VM_MAP, pgprot);
		size_t to_copy = PAGE_SIZE - offset;

		to_copy = min(to_copy, size);
		if (!vaddr) {
			ret = -ENOMEM;
			goto err;
		}

		if (write)
			ret = copy_from_user(vaddr + offset, ptr, to_copy);
		else
			ret = copy_to_user(ptr, vaddr + offset, to_copy);

		vunmap(vaddr);
		if (ret) {
			ret = -EFAULT;
			goto err;
		}
		size -= to_copy;
		if (!size)
			break;
		ptr += to_copy;
		offset = 0;
	}
*/
err:
	dma_buf_unmap_attachment(attach, table, dir);
	dma_buf_detach(dma_buf, attach);
	return ret;
}

static int ion_handle_test_kernel(struct dma_buf *dma_buf, void __user *ptr,
				  size_t offset, size_t size, bool write)
{
	int ret;
	unsigned long page_offset = offset >> PAGE_SHIFT;
	size_t copy_offset = offset % PAGE_SIZE;
	size_t copy_size = size;
	enum dma_data_direction dir = write ? DMA_FROM_DEVICE : DMA_TO_DEVICE;

	if (offset > dma_buf->size || size > dma_buf->size - offset)
		return -EINVAL;

	ret = dma_buf_begin_cpu_access(dma_buf, dir); //  invalidate cache, get latest data in DDR
	if (ret)
		return ret;

	while (copy_size > 0) {
		size_t to_copy;
		void *vaddr = dma_buf_kmap(dma_buf, page_offset);

		if (!vaddr)
			goto err;

		to_copy = min_t(size_t, PAGE_SIZE - copy_offset, copy_size);

		if (write)
			ret = copy_from_user(vaddr + copy_offset, ptr, to_copy);
		else
			ret = copy_to_user(ptr, vaddr + copy_offset, to_copy); // use the latest data

		dma_buf_kunmap(dma_buf, page_offset, vaddr);
		if (ret) {
			ret = -EFAULT;
			goto err;
		}

		copy_size -= to_copy;
		ptr += to_copy;
		page_offset++;
		copy_offset = 0;
	}
err:
	dma_buf_end_cpu_access(dma_buf, dir); // flush cache 
	return ret;
}

static long ion_test_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("ion_test_ioctl...\n");
	struct ion_test_data *test_data = filp->private_data;
	int ret = 0;

	union {
		struct ion_test_rw_data test_rw;
	} data;

	if (_IOC_SIZE(cmd) > sizeof(data))
		return -EINVAL;

	if (_IOC_DIR(cmd) & _IOC_WRITE)
		if (copy_from_user(&data, (void __user *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;
	printk("ion_test_ioctl1...\n");
	switch (cmd) {
	case ION_IOC_TEST_SET_FD:
	{
		struct dma_buf *dma_buf = NULL;
		int fd = arg;

		if (fd >= 0) {
			dma_buf = dma_buf_get((int)arg);
			if (IS_ERR(dma_buf))
				return PTR_ERR(dma_buf);
		}
		if (test_data->dma_buf)
			dma_buf_put(test_data->dma_buf);
		test_data->dma_buf = dma_buf;
		printk("ion_test_ioctl2...\n");
		break;
	}
	// 返回这块内存的DMA物理地址
	case ION_IOC_TEST_DMA_MAPPING:
	{
		printk("ion_test_ioctl1...\n");
		ret = ion_handle_test_dma(test_data->dev, test_data->dma_buf,
					  u64_to_uptr(data.test_rw.ptr),
					  data.test_rw.offset,
					  data.test_rw.size,
					  data.test_rw.write);
		break;
	}
	// 返回这块内存的逻辑地址
	case ION_IOC_TEST_KERNEL_MAPPING:
	{
		ret = ion_handle_test_kernel(test_data->dma_buf,
					     u64_to_uptr(data.test_rw.ptr),
					     data.test_rw.offset,
					     data.test_rw.size,
					     data.test_rw.write);
		break;
	}
	default:
		return -ENOTTY;
	}

	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (copy_to_user((void __user *)arg, &data, sizeof(data)))
			return -EFAULT;
	}
	return ret;
}


static struct  file_operations pciecom_ops = {
	.owner = THIS_MODULE,
	.open = pciecom_open,
	.release = pciecom_release,
	.read = pciecom_read,
	.write = pciecom_write,
	.unlocked_ioctl = ion_test_ioctl,
	.compat_ioctl = ion_test_ioctl,
};

static int __init ion_test_probe(struct platform_device *pdev)
{
	int ret;
	struct ion_test_device *testdev;

	testdev = devm_kzalloc(&pdev->dev, sizeof(struct ion_test_device),
			       GFP_KERNEL);
	if (!testdev)
		return -ENOMEM;

	testdev->misc.minor = MISC_DYNAMIC_MINOR;
	testdev->misc.name = "pciecom";
	testdev->misc.fops = &pciecom_ops;
	testdev->misc.parent = &pdev->dev;
	ret = misc_register(&testdev->misc);
	if (ret) {
		pr_err("failed to register misc device.\n");
		return ret;
	}

	platform_set_drvdata(pdev, testdev);

	return 0;
}

static int ion_test_remove(struct platform_device *pdev)
{
	struct ion_test_device *testdev;

	testdev = platform_get_drvdata(pdev);
	if (!testdev)
		return -ENODATA;

	misc_deregister(&testdev->misc);
	return 0;
}

static struct platform_device *ion_test_pdev;
static struct platform_driver ion_test_platform_driver = {
	.remove = ion_test_remove,
	.driver = {
		.name = "pciecom",
	},
};


static int __init pciecom_init(void) {
	printk("pciecom_init1\n");
// 	int ret;
// 	dev_t devno;
// 	devno = MKDEV(VERS_MAJOR, VERS_MINOR);
// 	ret = register_chrdev_region(devno, VERS_DEV_CNT, "pciecom_dev");
// 	// ret = alloc_chrdev_region(&devno, 0, 1, "pciecom_dev");
// 	if (ret)
// 		goto reg_error;
	
// 	printk("pciecom_init2\n");
	
// 	cdev_init(&comdev, &pciecom_ops);
// 	comdev.owner = THIS_MODULE;

// 	ret = cdev_add(&comdev, devno, VERS_DEV_CNT);
// 	if (ret)
// 		goto add_error;
	
// 	printk("pciecom_init3\n");
	
// 	return 0;

// add_error:
// 	printk("cdev_add failed\n");
// 	unregister_chrdev_region(devno, VERS_DEV_CNT);
// reg_error:
// 	printk("register_chrdev_region failed\n");
// 	return ret;

	ion_test_pdev = platform_device_register_simple("pciecom",
							-1, NULL, 0);
	if (IS_ERR(ion_test_pdev))
		return PTR_ERR(ion_test_pdev);

	return platform_driver_probe(&ion_test_platform_driver, ion_test_probe);
}


static void __exit pciecom_exit(void) {
	printk("pciecom_exit\n");
	// dev_t devno;
	// devno = MKDEV(VERS_MAJOR, VERS_MINOR);
	// cdev_del(&comdev);
	// unregister_chrdev_region(devno, VERS_DEV_CNT);
	platform_driver_unregister(&ion_test_platform_driver);
	platform_device_unregister(ion_test_pdev);
}

module_init(pciecom_init);
module_exit(pciecom_exit);

MODULE_AUTHOR("kexin.he@bst.ai");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A1000 comtest pcie rc and ep");
MODULE_ALIAS("comtest pcie");

