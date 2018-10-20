#include "gpio.h"
#include <stdio.h>     // printf()
#include <stdlib.h>    // atoi()
#include <string.h>    // strlen()
#include <sys/types.h>
#include <sys/stat.h>  // mkfifo()
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), close()

#define DEBUG true

bool openGpio(int port) {
    int deviceFD = open("/sys/class/gpio/export", O_WRONLY);
    if (deviceFD == -1) {
        printf("[OpenGpio] export gpio%d failed\n", port);
        return false;
    }
    char portValue[8] = {0};
    sprintf(portValue, "%d", port);
    write(deviceFD, portValue, strlen(portValue));
    close(deviceFD);
    usleep(50 * 1000);
#if DEBUG
    printf("[OpenGpio] open gpio%d\n", port);
#endif
    return true;
}

bool closeGpio(int port) {
    int deviceFD = open("/sys/class/gpio/unexport", O_WRONLY);
    if (deviceFD == -1) {
        printf("[CloseGpio] unexport gpio%d failed\n", port);
        return false;
    }
    char portValue[8] = {0};
    sprintf(portValue, "%d", port);
    write(deviceFD, portValue, strlen(portValue));
    close(deviceFD);
#if DEBUG
    printf("[CloseGpio] close gpio%d\n", port);
#endif
    return true;
}

bool setupGpioDirection(int port, int direction) {
    char deviceNode[256] = {0};
    sprintf(deviceNode, "/sys/class/gpio/gpio%d/direction", port);
    int deviceFD = open(deviceNode, O_WRONLY);
    if (deviceFD == -1) {
        printf("[SetupGpioDirection] setup the direction of gpio%d failed!\n", port);
        return false;
    }

    char gpioValue[8] = {0};
    sprintf(gpioValue, "%s", (direction == GPIO_INPUT) ? "in" : "out");
    write(deviceFD, gpioValue, strlen(gpioValue));
    close(deviceFD);
#if DEBUG
    printf("[SetupGpioDirection] setup gpio%d direction as %s\n", port, (direction == GPIO_INPUT) ? "IN" : "OUT");
#endif
    return true;
}

bool setupGpioEdge(int port, int edge) {
    char deviceNode[256] = {0};
    sprintf(deviceNode, "/sys/class/gpio/gpio%d/edge", port);
    int deviceFD = open(deviceNode, O_WRONLY);
    if (deviceFD == -1) {
        printf("[SetupGpioDirection] setup the edge of gpio%d failed!\n", port);
        return false;
    }

    char gpioValue[8] = {0};
    sprintf(gpioValue, "%s", (edge == GPIO_RAISE) 
                                ? "raising"
                                : (edge == GPIO_FALL) ? "falling" : "both");
    write(deviceFD, gpioValue, strlen(gpioValue));
    close(deviceFD);
#if DEBUG
    printf("[setupGpioEdge] setup gpio%d edge as %s\n", port, 
                             (edge == GPIO_RAISE) 
                                ? "raising"
                                : (edge == GPIO_FALL) ? "falling" : "both");
#endif
    return true;
}

bool setupGpioValue(int port, int value) {
    char deviceNode[256] = {0};
    sprintf(deviceNode, "/sys/class/gpio/gpio%d/value", port);
    int deviceFD = open(deviceNode, O_WRONLY);
    if (deviceFD == -1) {
        printf("[SetupGpioValue] setup the value of gpio%d failed!\n", port);
        return false;
    }

    char gpioValue[8] = {0};
    sprintf(gpioValue, "%d", value);
    write(deviceFD, gpioValue, strlen(gpioValue));
    close(deviceFD);
#if DEBUG
    printf("[SetupGpioValue] setup gpio%d value as %s\n", port, (value == GPIO_LOW) ? "LOW" : "HIGH");
#endif
    return true;
}

int readGpioValue(int port) {
    char deviceNode[256] = {0};
    sprintf(deviceNode, "/sys/class/gpio/gpio%d/value", port);
    int deviceFD = open(deviceNode, O_RDONLY);
    if (deviceFD == -1) {
        printf("[SetupGpioValue] setup the value of gpio%d failed!\n", port);
        return false;
    }

    char gpioValue[8] = {0};
    int count = read(deviceFD, gpioValue, 1);
    close(deviceFD);
#if DEBUG
    printf("[ReadGpioValue] the value of gpio %d is %s\n", count, deviceNode, port, gpioValue);
#endif
    return atoi(gpioValue);
}
