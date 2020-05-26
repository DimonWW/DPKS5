#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>

static LIST_HEAD(lab5_list_head);

struct time_keeper {
	ktime_t time_before;
	ktime_t time_after;
	struct list_head time_list;
};

static void print_text(unsigned int repeats)
{
	unsigned int repeat;
	struct time_keeper *ptr;
	for (repeat = 0; repeat < repeats; repeat++) {
		ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
		ptr->time_before = ktime_get();
		ptr->time_after = ktime_get();
		pr_info("Hello, world!\n");
		list_add(&ptr->time_list, &lab5_list_head);
	}
}

static unsigned int repeats = 1;
module_param(repeats, uint, 0444);
MODULE_PARM_DESC(repeats, "How many hello to print");

static int __init module4_init(void)
{
	if (repeats > 10) {
		pr_err("Cannot repeat more than 10 times\n");
		return -EINVAL;
	}
	if (repeats >= 5 && repeats <= 10)
		pr_warn("Repeatition from 5 to 10 times\n");
	if (repeats == 0)
		pr_warn("No repeatition\n");
	print_text(repeats);
	return 0;
}

static void __exit module4_exit(void)
{
	struct list_head *p;
	struct list_head *n;
	struct time_keeper *curr;
	pr_info("Module 4 exit\n");
	list_for_each_safe(p, n, &lab5_list_head) {
		curr = list_entry(p, struct time_keeper, time_list);
		pr_info("Before function time: %lld(ns).\n", 
				curr->time_after - curr->time_before);
		list_del(p);
		kfree(curr);
	}
}

module_init(module4_init);
module_exit(module4_exit);
MODULE_AUTHOR("Dmitry Korenko");
MODULE_DESCRIPTION("Training to work with list");
MODULE_LICENSE("Dual BSD/GPL");
