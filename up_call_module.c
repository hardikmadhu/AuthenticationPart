#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <asm/param.h>

struct timer_list exp_timer;

static void kill_passcode(unsigned long data)
{
	int err=0;
	char *argv[] = {"/home/hardik/Documents/project/Complete/kill_app",NULL};
	char *argv1[] = {"/home/hardik/Documents/project/Complete/term_restore",NULL};
		
	static char *envp[] = {
			        "HOME=/",
        			"TERM=linux",
        			"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };

	printk("Kill Passcode Called\n");

	err = call_usermodehelper( argv[0], argv, envp, 0);	
	if (err)
	{
		printk(KERN_INFO "Unable To execute 1\n");
		return;
	}
	
	printk("1st killed\n");
}

static int __init tst_init(void)
{	
	int delay = 300;
	
	printk(KERN_INFO "Init called\n");
	
	init_timer_on_stack(&exp_timer);
	
	exp_timer.expires = jiffies + delay * HZ;
	exp_timer.data = 0;
	exp_timer.function = kill_passcode;

	add_timer(&exp_timer);

	return 0;
}

static void __exit tst_exit(void)
{	
	del_timer(&exp_timer);	
	printk(KERN_INFO "Exit called\n");
}

module_init(tst_init);
module_exit(tst_exit);

MODULE_LICENSE("GPL");
