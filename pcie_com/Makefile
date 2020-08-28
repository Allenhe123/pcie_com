KERN_DIR=/home/allen/linux_kernel
#KERN_DIR=/root/driver/kernel

obj-m += pcie_com.o

all:
	make -C $(KERN_DIR) M=$(PWD) CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 O=build_out modules

clean:
	make -C $(KERN_DIR) M=$(PWD) CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 O=build_out modules clean
