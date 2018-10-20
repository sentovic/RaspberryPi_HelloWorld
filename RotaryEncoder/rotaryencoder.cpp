#include "rotaryencoder.h"
#include "../Gpio/gpio.h"
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <errno.h>
#include <fcntl.h>  // O_WRONLY, O_RDONLY
#include <unistd.h> // read(), close()
#include <poll.h>
#include <memory.h> // memset()

#define ROTARY_ENCODER_PIN_BTN 0
#define ROTARY_ENCODER_PIN_CLK 5
#define ROTARY_ENCODER_PIN_DAT 6

/**
 * Build:
 *     gcc -c gpio.c -o gpio.o
 *     gcc -c rotaryencoder.cpp -o rotaryencoder.o -lstdc++ -lpthread
 *     gcc rotaryencoder.o gpio.o -lstdc++ -o rotaryencoder.exe
 *
 * mkdir obj
 * gcc -c ../Gpio/gpio.c -o ./obj/gpio.o
 * gcc -c rotaryencoder.cpp -o ./obj/rotaryencoder.o -lstdc++ -lpthread
 * gcc ./obj/rotaryencoder.o ./obj/gpio.o -lstdc++ -lpthread -o rotaryencoder.exe
 * rm -rf ./obj
 */
int main() {
    RotaryEncoder* encoder = new RotaryEncoder(
        ROTARY_ENCODER_PIN_CLK, ROTARY_ENCODER_PIN_DAT, ROTARY_ENCODER_PIN_BTN
    );
    encoder->enable();

    pthread_t* waittingThread = encoder->getListenThread()->getThread();
    pthread_join(*waittingThread, NULL);
    return 0;
}

static void* run(void* param) {
    ThreadWrapper* wrapper = static_cast<ThreadWrapper*>(param);
    if (wrapper == NULL) {
        return NULL;
    }
    wrapper->getRotaryEncoder()->openGpios();

    const int PIN_CLK_FD = wrapper->getRotaryEncoder()->getPinFD(RotaryEncoder::PIN_CLOCK);
    const int PIN_DAT_FD = wrapper->getRotaryEncoder()->getPinFD(RotaryEncoder::PIN_DATA);
    const int PIN_BTN_FD = wrapper->getRotaryEncoder()->getPinFD(RotaryEncoder::PIN_BUTTON);

    const int PIN_CLK = 0;
    const int PIN_DAT = 1;
    const int PIN_BTN = 2;
    const int POLL_TIMEOUT = -1;
    const int POLL_FD_COUNT = (PIN_BTN_FD == -1) ? 2 : 3;

    if ((PIN_CLK_FD == -1) || (PIN_DAT_FD == -1)) {
        printf("[ThreadWrapper::run] the fds of pin CLK/DAT is invalid, cancel gpio listen\n");
        return NULL;
    }

    int valueClk = -1;
    int valueDat = -1;
    int valueBtn = -1;

    volatile int  rotaryEncoderLastPulse    = 0;
    volatile int  rotaryEncoderPulse        = 0;
    volatile int  lastestRotaryEncoderPinAB = 1; // last last pin value of A and B
    volatile int  lastRotaryEncoderPinAB    = 1; // last pin value of A and B

    const int BUFFER_SIZE = 8;
    char clkBuffer[BUFFER_SIZE] = {0};
    char datBuffer[BUFFER_SIZE] = {0};
    char btnBuffer[BUFFER_SIZE] = {0};
    struct pollfd pollFds[POLL_FD_COUNT];
    while (!wrapper->isInterruptted()) {
        memset(pollFds, 0, sizeof(struct pollfd) * POLL_FD_COUNT);
        pollFds[PIN_CLK].fd     = PIN_CLK_FD;
        pollFds[PIN_CLK].events = POLLPRI;
        pollFds[PIN_DAT].fd     = PIN_DAT_FD;
        pollFds[PIN_DAT].events = POLLPRI;
        if (PIN_BTN_FD != -1) {
            pollFds[PIN_BTN].fd     = PIN_BTN_FD;
            pollFds[PIN_BTN].events = POLLPRI;
        }

        int result = poll(pollFds, POLL_FD_COUNT, POLL_TIMEOUT);
        if (result > 0) { // Have {result} results
            bool isCLKActive = (((pollFds[PIN_CLK].revents & pollFds[PIN_CLK].events) != 0));
            bool isDATActive = (((pollFds[PIN_DAT].revents & pollFds[PIN_DAT].events) != 0));
            if (isCLKActive || isDATActive) {
                memset(clkBuffer, 0, BUFFER_SIZE);
                read(pollFds[PIN_CLK].fd, clkBuffer, BUFFER_SIZE);
                lseek(pollFds[PIN_CLK].fd, 0, SEEK_SET);
                valueClk = atoi(clkBuffer);

                memset(datBuffer, 0, BUFFER_SIZE);
                read(pollFds[PIN_DAT].fd, datBuffer, BUFFER_SIZE);
                lseek(pollFds[PIN_DAT].fd, 0, SEEK_SET);
                valueDat = atoi(datBuffer);

                // (1<=>1) is rotary encoder start position
                // (1<=>0) is start to rotate clockwise, or (0<=>1) is start to rotate re-clockwise
                // (0<=>0) is middle position
                // (0<=>1) is end to rotate clockwise, or (1<=>0) is start to rotate re-clockwise
                // (1<=>1) is rotary encoder end position
                int currentPin = valueClk * 10 + valueDat;
                if (currentPin != lastRotaryEncoderPinAB) {
                    if (lastRotaryEncoderPinAB == 00) {
                        if (lastestRotaryEncoderPinAB == 10 && currentPin == 01) rotaryEncoderPulse++;
                        else if (lastestRotaryEncoderPinAB == 01 && currentPin == 10) rotaryEncoderPulse--;
                    }

                    lastestRotaryEncoderPinAB = lastRotaryEncoderPinAB;
                    lastRotaryEncoderPinAB = currentPin;
                }

                if (rotaryEncoderLastPulse != rotaryEncoderPulse) {
                    rotaryEncoderLastPulse = rotaryEncoderPulse;
                }
            }

            if ((PIN_BTN_FD != -1) &&
                ((pollFds[PIN_BTN].revents & pollFds[PIN_BTN].events) != 0) ) {
                memset(btnBuffer, 0, BUFFER_SIZE);
                read(pollFds[PIN_BTN].fd, btnBuffer, BUFFER_SIZE);
                lseek(pollFds[PIN_BTN].fd, 0, SEEK_SET);
                valueBtn = atoi(btnBuffer);
            }

            printf("[ThreadWrapper::run] poll %d events: CLK %s, DAT %s, BTN %s, pulse is %d\n",
                    result,
                    (valueClk == 0) ? "LOW" : ((valueClk == 1) ? "HIGH" : "NONE"),
                    (valueDat == 0) ? "LOW" : ((valueDat == 1) ? "HIGH" : "NONE"),
                    (valueBtn == 0) ? "LOW" : ((valueBtn == 1) ? "HIGH" : "NONE"),
                    rotaryEncoderLastPulse);

        } else if (result < 0) { // Error occurred
            printf("[ThreadWrapper::run] poll failed, error is %d\n", result);
        } else { // No results
            printf("[ThreadWrapper::run] poll no result, continue...\n");
        }
    }

    wrapper->getRotaryEncoder()->closeGpios();
    pthread_exit(NULL);
}

void ThreadWrapper::start() {
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);

    int result = pthread_create(mThread, &threadAttr, run, this);
    if (result != 0) {
        printf("[ThreadWrapper::start] thread create failed, cannot start!\n");
    }
}

RotaryEncoder::RotaryEncoder(int portCLK, int portDAT) {
    mPortCLK   = portCLK;
    mPortDAT   = portDAT;
    mPortBTN   = -1;
    mPortCLKFD = -1;
    mPortDATFD = -1;
    mPortBTNFD = -1;
    mListenThread = NULL;
}

RotaryEncoder::RotaryEncoder(int portCLK, int portDAT, int portBTN) {
    mPortCLK   = portCLK;
    mPortDAT   = portDAT;
    mPortBTN   = portBTN;
    mPortCLKFD = -1;
    mPortDATFD = -1;
    mPortBTNFD = -1;
    mListenThread = NULL;
}

bool RotaryEncoder::openGpios() {
    if (mPortCLK != -1) {
        printf("[openGpios] mPortCLK = -1\n");
        openGpio(mPortCLK);
        setupGpioDirection(mPortCLK, GPIO_INPUT);
        setupGpioEdge(mPortCLK, GPIO_BOTH);

        char deviceNode[256] = {0};
        sprintf(deviceNode, "/sys/class/gpio/gpio%d/value", mPortCLK);
        int deviceFD = open(deviceNode, O_RDONLY);
        if (deviceFD == -1) {
            printf("[openGpios] setup the value of gpio%d failed!\n", mPortCLK);
            return false;
        }
        printf("[openGpios] mPortCLKFD = deviceFD(%d)\n", deviceFD);
        mPortCLKFD = deviceFD;
    }

    if (mPortDAT != -1) {
        printf("[openGpios] mPortDAT = -1\n");
        openGpio(mPortDAT);
        setupGpioDirection(mPortDAT, GPIO_INPUT);
        setupGpioEdge(mPortDAT, GPIO_BOTH);

        char deviceNode[256] = {0};
        sprintf(deviceNode, "/sys/class/gpio/gpio%d/value", mPortDAT);
        int deviceFD = open(deviceNode, O_RDONLY);
        if (deviceFD == -1) {
            printf("[openGpios] setup the value of gpio%d failed!\n", mPortDAT);
            return false;
        }
        printf("[openGpios] mPortDATFD = deviceFD(%d)\n", deviceFD);
        mPortDATFD = deviceFD;
    }

    if (mPortBTN != -1) {
        printf("[openGpios] mPortBTN = -1\n");
        openGpio(mPortBTN);
        setupGpioDirection(mPortBTN, GPIO_INPUT);
        setupGpioEdge(mPortBTN, GPIO_BOTH);

        char deviceNode[256] = {0};
        sprintf(deviceNode, "/sys/class/gpio/gpio%d/value", mPortBTN);
        int deviceFD = open(deviceNode, O_RDONLY);
        if (deviceFD == -1) {
            printf("[openGpios] setup the value of gpio%d failed!\n", mPortBTN);
            return false;
        }
        printf("[openGpios] mPortBTNFD = deviceFD(%d)\n", deviceFD);
        mPortBTNFD = deviceFD;
    }
}

bool RotaryEncoder::closeGpios() {
    if (mPortCLK == -1) {
        close(mPortCLKFD);
        mPortCLKFD = -1;
        closeGpio(mPortCLK);
    }

    if (mPortDAT == -1) {
        close(mPortDATFD);
        mPortDATFD = -1;
        closeGpio(mPortDAT);
    }

    if (mPortBTN == -1) {
        close(mPortBTNFD);
        mPortBTNFD = -1;
        closeGpio(mPortBTN);
    }
}

void RotaryEncoder::enable() {
    if (mListenThread != NULL) {
        stopListenThread();
    }

    startListenThread();
}

void RotaryEncoder::startListenThread() {
    mListenThread = new ThreadWrapper(this);
    mListenThread->start();
}

void RotaryEncoder::stopListenThread() {
    mListenThread->interrupt();
}

