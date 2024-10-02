#include <linux/module.h>      // Required for all kernel modules
#include <linux/kernel.h>      // KERN_INFO, etc.
#include <linux/fs.h>          // file_operations
#include <linux/device.h>      // device_create, class_create, etc.
#include <linux/uaccess.h>     // copy_to_user, copy_from_user

#define DEVICE_NAME "chardev"
#define CLASS_NAME  "chardev_class"

static int major_number;
static struct class* chardev_class = NULL;
static struct device* chardev_device = NULL;

// Function prototypes for the file operations
static ssize_t dev_read(struct file* file, char __user* buffer, size_t len, loff_t* offset);
static ssize_t dev_write(struct file* file, const char __user* buffer, size_t len, loff_t* offset);

// Define the file operations structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
};

// Module initialization function
static int __init chardev_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        return major_number;
    }

    chardev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create class\n");
        return PTR_ERR(chardev_class);
    }

    chardev_device = device_create(chardev_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(chardev_device)) {
        class_destroy(chardev_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create device\n");
        return PTR_ERR(chardev_device);
    }

    printk(KERN_INFO "Character device registered: %s with major number %d\n", DEVICE_NAME, major_number);
    return 0;
}

// Module exit function
static void __exit chardev_exit(void) {
    device_destroy(chardev_class, MKDEV(major_number, 0));
    class_unregister(chardev_class);
    class_destroy(chardev_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Character device unregistered: %s\n", DEVICE_NAME);
}

static ssize_t dev_read(struct file* file, char __user* buffer, size_t len, loff_t* offset) {
    // Example read implementation
    return 0; // Return the number of bytes read
}

static ssize_t dev_write(struct file* file, const char __user* buffer, size_t len, loff_t* offset) {
    // Example write implementation
    return len; // Return the number of bytes written
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character driver");
MODULE_VERSION("1.0");

