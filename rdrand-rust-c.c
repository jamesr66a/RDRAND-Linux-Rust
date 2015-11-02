#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/bug.h>

void *malloc(size_t s) { return kmalloc(s, GFP_KERNEL); }

void free(void *ptr) { kfree(ptr); }

extern ssize_t rdrand_read(struct file *, char __user *, size_t, loff_t *);
extern void test_fn(void);

static const struct file_operations rdrand_fops = { read : rdrand_read };

static int major;

static struct miscdevice rdrand_dev = { MISC_DYNAMIC_MINOR, "rdrand",
                                        &rdrand_fops };

static int hello_init(void) {
  printk(KERN_INFO "hello: init\n");
  major = misc_register(&rdrand_dev);
  if (major < 0) {
    printk(KERN_ALERT "Registering hello char device failed: %i\n", major);
    return major;
  } else {
    printk(KERN_INFO "Registering hello char device suceeded: %i\n", major);
  }
  return 0;
}

static void hello_exit(void) {
  printk(KERN_INFO "hello:exit \n");
  misc_deregister(&rdrand_dev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("MIT");
