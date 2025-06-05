#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x52530597, "cpufreq_register_governor" },
	{ 0xc1c48d1e, "__cpufreq_driver_target" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xcd91b127, "system_highpri_wq" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0xf0062efa, "get_cpu_util" },
	{ 0x6609d9c5, "get_loadavg_percent" },
	{ 0xae2ccbb2, "get_cpu_temperature_millicelsius" },
	{ 0x5928ab72, "get_run_queue_length" },
	{ 0x122c3a7e, "_printk" },
	{ 0x37a0cba, "kfree" },
	{ 0x49a8c60b, "cpufreq_unregister_governor" },
	{ 0x3142be5e, "kmalloc_caches" },
	{ 0x323febe6, "__kmalloc_cache_noprof" },
	{ 0xa16b9ad9, "gov_attr_set_init" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "cpufreq_log_feature");


MODULE_INFO(srcversion, "6AD7CFC9AD2C6C8840A80C1");
