obj-m += test.o cpufreq_modelbased.o 

test-objs := cpufreq_log_feature.o
cpufreq_modelbased-objs := cpufreq_log_feature.o model_inference.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean