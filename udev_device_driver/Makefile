obj-m += udev_class_mknod.o
 
KERNEL_DIR ?= $(HOME)/boot_practice/linux

all:
	 make -C $(KERNEL_DIR) \
       ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
       SUBDIRS=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
       ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
       SUBDIRS=$(PWD) clean

deploy:
	scp *.ko pi@192.168.2.69:

#obj-m := udev_class_mknod.o
#
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
#
