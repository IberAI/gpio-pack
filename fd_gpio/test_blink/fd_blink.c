#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#define GPIO_BASE 0x3F200000  // Base address for GPIO controller on BCM2837
#define BLOCK_SIZE (4*1024)  // Size of the memory block to map

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |= (1<<(((g)%10)*3))
#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)

volatile unsigned *gpio;  // Pointer to the mapped GPIO control registers

void setup_io();
int pin_ready(int pin, bool mode);
int pin_toggle(int pin, bool set);
void handle_error(const char *message);

int main(int argc, char **argv)
{
    setup_io();

    // Prepare GPIO 18 and toggle it
    if (pin_ready(18, false) == 0) {  // Set pin 18 as output
        pin_toggle(18, true);  // Set pin 18
        sleep(1);
        pin_toggle(18, false);  // Clear pin 18
    }

    return 0;
}

void setup_io()
{
    int mem_fd;
    void *gpio_map;

    if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) < 0) {
        handle_error("Failed to open /dev/gpiomem");
    }

    gpio_map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        handle_error("Failed to map GPIO memory");
    }

    gpio = (volatile unsigned *)gpio_map;
}

int pin_ready(int pin, bool mode)
{
    if (pin < 0 || pin > 27) {
        printf("Error: GPIO pin %d is out of valid range (0-27).\n", pin);
        return -1;
    }

    INP_GPIO(pin);  // Set pin as input
    if (!mode) {
        OUT_GPIO(pin);  // Set pin as output
    }

    return 0;
}

int pin_toggle(int pin, bool set)
{
    if (set) {
        GPIO_SET = 1 << pin;
    } else {
        GPIO_CLR = 1 << pin;
    }
    return set ? 1 : 0;
}

void handle_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

