# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := ase_kmod.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /home/arctarus/Documents/fac/master/s2/ase++/linux3.19/linux-3.19.1
	BUILDDIR  ?= /home/arctarus/Documents/fac/master/s2/ase++/linux3.19/build/kvm32
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) O=$(BUILDDIR) M=$(PWD) modules
endif
