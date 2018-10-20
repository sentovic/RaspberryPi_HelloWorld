#ifndef GPIO_H
#define GPIO_H

#define GPIO_HIGH   1
#define GPIO_LOW    0
#define GPIO_INPUT -1
#define GPIO_OUTPUT 1
#define GPIO_RAISE -1
#define GPIO_BOTH   0
#define GPIO_FALL   1

#include <stdbool.h>   // bool

#ifdef __cplusplus
extern "C" {
#endif

    bool openGpio(int port);
    bool setupGpioDirection(int port, int direction);
    bool setupGpioValue(int port, int value);
    bool setupGpioEdge(int port, int edge);
    int  readGpioValue(int port);
    bool closeGpio(int port);

#ifdef __cplusplus
}
#endif

#endif
