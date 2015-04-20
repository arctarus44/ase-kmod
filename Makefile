# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := asekmod.o
	asekmod-y := pid_list.o ase_kmod.o
	CFLAGS_ase_kmod.o := -DDEBUG
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /gfs/monbailly/linux-3.19.1
	BUILDDIR  ?= /gfs/monbailly/build/kvm32
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) O=$(BUILDDIR) M=$(PWD) modules
endif
