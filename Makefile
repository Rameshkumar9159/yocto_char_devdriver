obj-m := chardevdriver.o

KDIR := /lib/modules/$(shell uname -r)/build

TDIR := /home/rameshchlm/BSP_BOOT/linux-rba5d2x

host:
	make -C $(KDIR) M=$(PWD) modules

target:
	make -C $(TDIR) M=$(PWD) modules

clean:
	make -C $(TDIR) M=$(PWD) clean
	make -C $(KDIR) M=$(PWD) clean



