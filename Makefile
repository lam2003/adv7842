export AR = arm-hisiv500-linux-gcc
export ARCH = arm
export CC = arm-hisiv500-linux-gcc
export EXTRA_CFLAGS += $(DEBFLAGS) -Wall
export EXTRA_CFLAGS += -I$(LDDINC)
export LD = arm-hisiv500-linux-ld
ifneq ($(KERNELRELEASE),)

obj-m := adv7842drv.o
adv7842drv-objs += adv7842.o rx_detect.o
else
KERNELDIR ?= /win/linux-3.0.y
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD)/../include modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD)/../include clean
endif

