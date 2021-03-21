#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mydev"
#define CLASS_NAME "dev"
#define DRIVER_AUTHOR "Dinh Cong Toan"
#define DRIVER_DESC   "A sample character device driver"
#define DRIVER_VERSION "0.1"

struct vchar_dev {
    dev_t dev_num;
    struct class *dev_class;
    struct class cdev;
    struct device *dev;
} vchar_drv;

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
    return 0;
failed_register_device_node:
    class_destroy(vchar_drv.dev_class);
failed_register_class_device:
    unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum: 
    return ret;
}

static void __exit vchar_driver_exit(void) {
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
