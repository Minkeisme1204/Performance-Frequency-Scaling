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
	{ 0xf0062efa, "get_cpu_util" },
	{ 0x6609d9c5, "get_loadavg_percent" },
	{ 0xae2ccbb2, "get_cpu_temperature_millicelsius" },
	{ 0x5928ab72, "get_run_queue_length" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "cpufreq_log_feature");


MODULE_INFO(srcversion, "303C491F5120300E40CB314");
