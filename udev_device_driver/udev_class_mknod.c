#include <linux/module.h>   // declare module_init() and module_exit()
#include <linux/fs.h>       // define func to register and unregister device number 
#include <linux/device.h>   // define func to create and ddestroy device file
#include <linux/cdev.h>     // 
#include <linux/slab.h>     // allocate memory in kernel space, kmalloc(N, GFP_KERNEL) and kzalloc
#include "udev_class_mknod.h"

#define DEVICE_NAME "mydev"
#define CLASS_NAME "dev"
#define DRIVER_AUTHOR "Dinh Cong Toan"
#define DRIVER_DESC   "A sample character device driver"
#define DRIVER_VERSION "0.1"

typedef struct vchar_dev {
    unsigned char *control_regs;
    unsigned char *status_regs;
    unsigned char *data_regs;
} vchar_dev_t;

struct vchar_drv {
    dev_t dev_num;
    struct class *dev_class;
    struct class cdev;
    struct device *dev;
    vchar_dev_t *vchar_hw;
} vchar_drv;

/****************** device specific - START *******************/

/* init the device */
int vchar_hw_init(vchar_dev_t *hw) {
    char *buf;
    buf = kzalloc(NUM_DEV_REGS * REG_SIZE, GFP_KERNEL);
    if (!buf) {
        return -ENOMEM;
    }

    hw->control_regs = buf;
    hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
    hw->data_regs = hw->status_regs + NUM_STS_REGS;

    /* init data register */
    hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
    hw->status_regs[DEVICE_STATUS_REG] = 0x03;
    return 0;
}

/* exit the device*/
int vchar_hw_exit(vchar_dev_t *hw) {
    kfree(hw->control_regs);    
}

/* register driver */
static int __init vchar_driver_init(void) {
    int ret = 0;
    /* dynamic allocate device number */ 
    vchar_drv.dev_num = 0;
    ret = alloc_chrdev_region(&vchar_drv.dev_num, 0, 1, "class_vchar_dev");
    if (ret < 0) {
        printk("Failed to register device number dynamic");
        goto failed_register_devnum;
    }
    printk("Allocate device number (%d, %d\n", MAJOR(vchar_drv.dev_num), MINOR(vchar_drv.dev_num));

    /* register device class */
    vchar_drv.dev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (vchar_drv.dev_class == NULL) {
        printk("Failed to initialize class device\n");
        goto failed_register_class_device;
    }
    /* create device file or device node named DEVICE_NAME associated a dev */
    vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(vchar_drv.dev)) {
        printk("Failed to initialize device node\n");
        goto failed_register_device_node;
    }
    printk("initialize vchar drive successfully");
    
    /* allocate memory for data structure of driver */
    vchar_drv.vchar_hw = kmalloc(sizeof(vchar_dev_t), GFP_KERNEL);
    if (!vchar_drv.vchar_hw) {
        printk("Failed to allocate data structure of driver\n");
        ret = -ENOMEM;
        goto failed_allocate_structure;
    }
    
    /* init the physical device */
    ret = vchar_hw_init(vchar_drv.vchar_hw);
    if (ret < 0) {
        printk("Failed to initialize a virtual character device\n");
        goto failed_init_hw;
    }

    return 0;


failed_init_hw:
    kfree(vchar_drv.vchar_hw);   
failed_allocate_structure:
    device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
failed_register_device_node:
    class_destroy(vchar_drv.dev_class);
failed_register_class_device:
    unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum: 
    return ret;
}

static void __exit vchar_driver_exit(void) {
    /* exit physical device */
    vchar_hw_exit(vchar_drv.vchar_hw);
    /* free kernel buffer*/
    kfree(vchar_drv.vchar_hw);
    /* unregister device node */
    device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
    /* unregister device class */
    class_destroy(vchar_drv.dev_class);
    /* unregister device number */
    unregister_chrdev_region(vchar_drv.dev_num, 1);
    printk("exit vchar driver\n ");
}

module_init(vchar_driver_init);
module_exit(vchar_driver_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR(DRIVER_AUTHOR); 
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION(DRIVER_VERSION); 
