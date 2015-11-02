#include <asm-generic/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>

static ssize_t device_read(struct file *file, char __user *user_buf, size_t len,
                           loff_t *off) {
  size_t i;
  char *buf = kzalloc(len, __GFP_WAIT);

  for (i = 0; i < len / sizeof(int); i++) {
    __asm__ __volatile__("rdrand %0\n" : "=r"((*(int*)&buf[i])));
  }

  if (len % sizeof(int)) {
    union {
      int i;
      char c[sizeof(int)];
    } remainder;
    __asm__ __volatile__("rdrand %0\n" : "=r"(remainder.i));
    for (i = 0; i < len % sizeof(int); i++) {
      buf[len / 4 + i] = remainder.c[i];
    }
  }

  copy_to_user(user_buf, buf, len);

  kfree(buf);

  return len;
}

static const struct file_operations rdrand_fops = { read : device_read };

static int major;

static struct miscdevice rdrand_dev = { MISC_DYNAMIC_MINOR, "rdrand",
                                        &rdrand_fops };

static int __init rdrand_init(void) {
  major = misc_register(&rdrand_dev);
  if (major < 0) {
    printk(KERN_ALERT "Registering hello char device failed: %i\n", major);
    return major;
  } else {
    printk(KERN_INFO "Registering hello char device suceeded: %i\n", major);
  }
  return 0;
}

static void __exit rdrand_exit(void) { misc_deregister(&rdrand_dev); }

module_init(rdrand_init);
module_exit(rdrand_exit);
