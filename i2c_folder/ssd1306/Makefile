obj-m += ssd1306.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

	gcc -o a main.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm a