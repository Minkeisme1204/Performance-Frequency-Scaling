#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

KSYMTAB_FUNC(get_cpu_util, "", "");
KSYMTAB_FUNC(get_loadavg_percent, "", "");
KSYMTAB_FUNC(get_cpu_temperature_millicelsius, "", "");
KSYMTAB_FUNC(get_run_queue_length, "", "");

SYMBOL_CRC(get_cpu_util, 0xf0062efa, "");
SYMBOL_CRC(get_loadavg_percent, 0x6609d9c5, "");
SYMBOL_CRC(get_cpu_temperature_millicelsius, 0xae2ccbb2, "");
SYMBOL_CRC(get_run_queue_length, 0x5928ab72, "");

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x341dbfa3, "__per_cpu_offset" },
	{ 0xb58aeaab, "kernel_cpustat" },
	{ 0xf1e98c74, "avenrun" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0xca9bf5, "init_task" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0xd59643f1, "thermal_zone_get_zone_by_name" },
	{ 0x93f55ce7, "thermal_zone_get_temp" },
	{ 0x122c3a7e, "_printk" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x921b07b1, "__cpu_online_mask" },
	{ 0x39ff040a, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4238328633701EA11BF817F");
