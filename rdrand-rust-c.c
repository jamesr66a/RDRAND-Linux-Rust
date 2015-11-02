#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/bug.h>

void *malloc(size_t s)
{
  return kmalloc(s, GFP_KERNEL);
}

void free(void *ptr)
{
  kfree(ptr);
}

extern void rust_init(void);
extern void rust_exit(void);

static int hello_init(void)
{
  printk(KERN_INFO "hello: init\n");
  rust_init();
  return 0;
}

static void hello_exit(void)
{
  rust_exit();
  printk(KERN_INFO "hello:exit \n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("MIT");
