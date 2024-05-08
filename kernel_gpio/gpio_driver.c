#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define IBR_MAX_USER_SIZE 1024

#define BCM2837_GPIO_ADDRESS 0x3F200000
#define BCM2711_GPIO_ADDRESS 0xfe200000

static struct proc_dir_entry *ibr_proc = NULL;
static char data_buffer[IBR_MAX_USER_SIZE + 1] = {0};
static unsigned int *gpio_registers = NULL;

static unsigned int get_gpio_base_address(void) {
    // Future enhancement: Choose address based on actual detected model.
    return BCM2837_GPIO_ADDRESS;  // Defaulting to BCM2837 for now
}

static void gpio_pin_on(unsigned int pin) {
    unsigned int fsel_index = pin / 10;
    unsigned int fsel_bitpos = (pin % 10) * 3;
    unsigned int* gpio_fsel = gpio_registers + fsel_index;
    unsigned int* gpio_set = gpio_registers + (0x1c / sizeof(unsigned int));

    *gpio_fsel &= ~(7 << fsel_bitpos);
    *gpio_fsel |= (1 << fsel_bitpos);
    *gpio_set = (1 << (pin % 32));
}

static void gpio_pin_off(unsigned int pin) {
    unsigned int* gpio_clr = gpio_registers + (0x28 / sizeof(unsigned int));
    *gpio_clr = (1 << (pin % 32));
}

static ssize_t ibr_read(struct file *file, char __user *user, size_t size, loff_t *off) {
    return simple_read_from_buffer(user, size, off, "Hello!\n", 7);
}

static ssize_t ibr_write(struct file *file, const char __user *user, size_t size, loff_t *off) {
    unsigned int pin, value;

    if (size > IBR_MAX_USER_SIZE) size = IBR_MAX_USER_SIZE;
    if (copy_from_user(data_buffer, user, size)) return -EFAULT;

    data_buffer[size] = '\0'; // Null-terminate the buffer
    if (sscanf(data_buffer, "%u,%u", &pin, &value) != 2) {
        printk(KERN_INFO "Improper data format submitted: %s\n", data_buffer);
        return -EINVAL;
    }

    if (pin > 53) {  // Check for valid GPIO pin on the Raspberry Pi
        printk(KERN_INFO "Invalid pin number submitted: %u\n", pin);
        return -EINVAL;
    }

    if (value != 0 && value != 1) {
        printk(KERN_INFO "Invalid value, use 0 or 1.\n");
        return -EINVAL;
    }

    if (value == 1) gpio_pin_on(pin);
    else gpio_pin_off(pin);

    printk(KERN_INFO "Set pin %u to %u\n", pin, value);
    return size;
}

static const struct proc_ops ibr_proc_fops = {
    .proc_read = ibr_read,
    .proc_write = ibr_write,
};

static int __init gpio_driver_init(void) {
    printk(KERN_INFO "Welcome to my driver!\n");

    gpio_registers = ioremap(get_gpio_base_address(), PAGE_SIZE);
    if (!gpio_registers) {
        printk(KERN_ERR "Failed to map GPIO memory to driver\n");
        return -ENOMEM;
    }

    ibr_proc = proc_create("ibr-gpio", 0666, NULL, &ibr_proc_fops);
    if (!ibr_proc) {
        iounmap(gpio_registers);
        return -ENOMEM;
    }

    printk(KERN_INFO "Successfully initialized GPIO driver\n");
    return 0;
}

static void __exit gpio_driver_exit(void) {
    printk(KERN_INFO "Unloading GPIO driver\n");
    iounmap(gpio_registers);
    proc_remove(ibr_proc);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("IberAI");
MODULE_DESCRIPTION("Enhanced GPIO control driver for Raspberry Pi");
MODULE_VERSION("0.1");

