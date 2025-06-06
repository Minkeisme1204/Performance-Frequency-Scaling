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
	{ 0x122c3a7e, "_printk" },
	{ 0x52530597, "cpufreq_register_governor" },
	{ 0xc1c48d1e, "__cpufreq_driver_target" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xcd91b127, "system_highpri_wq" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0x37a0cba, "kfree" },
	{ 0x49a8c60b, "cpufreq_unregister_governor" },
	{ 0x3142be5e, "kmalloc_caches" },
	{ 0x323febe6, "__kmalloc_cache_noprof" },
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0xb58aeaab, "kernel_cpustat" },
	{ 0x921b07b1, "__cpu_online_mask" },
	{ 0xf1e98c74, "avenrun" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0xca9bf5, "init_task" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0xd59643f1, "thermal_zone_get_zone_by_name" },
	{ 0x93f55ce7, "thermal_zone_get_temp" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xc60d0620, "__num_online_cpus" },
	{ 0xa0418439, "cpufreq_driver_target" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0ECBBF6C266ADA82B690CD1");
