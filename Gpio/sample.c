#include "gpio.h"
#include <stdio.h>     // printf()
#include <string.h>    // strlen()
#include <sys/types.h>
#include <sys/stat.h>  // mkfifo()
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), close()

#define DEBUG       true
#define KEYCODE_ESC 27
#define FLASH_TIME  50
#define SLEEP_TIME  4000

typedef struct {
    int port;
    int direction;
    int value;
} GPIO;

bool openGpio(int port);
bool setupGpioDirection(int port, int direction);
bool setupGpioValue(int port, int value);
bool closeGpio(int port);
void dumpGpioInfo();
int  getch();

/**
 * Build:
 *     gcc -o gpio.exe gpio.c 
 */
int main() {
    dumpGpioInfo();

    GPIO gpio = {
        .port      = 26,
        .direction = GPIO_OUTPUT,
        .value     = GPIO_LOW
    };
    if (!openGpio(gpio.port)) return 0;
    if (!setupGpioDirection(gpio.port, gpio.direction)) return 0;
    if (!setupGpioValue(gpio.port, gpio.value)) return 0;

    while (true) {
        setupGpioValue(gpio.port, GPIO_HIGH); usleep(FLASH_TIME * 1000);
        setupGpioValue(gpio.port, GPIO_LOW);  usleep(FLASH_TIME * 1000);
        setupGpioValue(gpio.port, GPIO_HIGH); usleep(FLASH_TIME * 1000);
        setupGpioValue(gpio.port, GPIO_LOW);  usleep(SLEEP_TIME * 1000);
    }

    if (!closeGpio(gpio.port)) return 0;
}

void dumpGpioInfo() {
    int labelFD = open("/sys/class/gpio/gpiochip0/label", O_RDONLY);
    if (labelFD == -1) {
         printf("[DumpGpioInfo] label read failed!\n");
    } else {
        char label[256] = {0};
        read(labelFD, label, 256);
        printf("[DumpGpioInfo] Gpio label: %s\n", label);
        close(labelFD);
        labelFD = -1;
    }
}
