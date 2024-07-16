obj-m += gpio_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod gpio_driver.ko

remove:
	sudo rmmod gpio_driver

reload:
	sudo rmmod gpio_driver || true
	sudo insmod gpio_driver.ko

status:
	lsmod | grep gpio_driver

info:
	modinfo gpio_driver.ko

