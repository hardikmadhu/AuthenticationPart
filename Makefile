obj-m := $(FILE).o

KDIR ?= /lib/modules/$(shell uname -r)/build
#KDIR ?= /home/hardik/Documents/omap3evm/linux-03.00.01.06

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

