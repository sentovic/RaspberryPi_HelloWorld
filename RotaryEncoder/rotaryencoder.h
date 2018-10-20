#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

class RotaryEncoder;

class ThreadWrapper {
public:
    ThreadWrapper(RotaryEncoder* encoder) {
        mEncoder = encoder;
        mThread = new pthread_t;
        mIsInterrupt = false;
    }

    ~ThreadWrapper() {
        delete mThread;
        mThread = NULL;
    }

    void start();

    inline void interrupt() {
        mIsInterrupt = true;
    }

    inline bool isInterruptted() {
        return mIsInterrupt;
    }

    inline pthread_t* getThread() {
        return mThread;
    }

    inline RotaryEncoder* getRotaryEncoder() {
        return mEncoder;
    }

private:
    RotaryEncoder* mEncoder;
    pthread_t* mThread;
    bool mIsInterrupt;
};

class RotaryEncoder {
public:
    static const int PIN_CLOCK  = 0;
    static const int PIN_DATA   = 1;
    static const int PIN_BUTTON = 2;
    
public:
    RotaryEncoder(int portCLK, int portDAT);
    RotaryEncoder(int portCLK, int portDAT, int portBTN);
    ~RotaryEncoder();

    void enable();
    bool openGpios();
    bool closeGpios();

    inline ThreadWrapper* getListenThread() {
        return mListenThread;
    }

    inline int getPinFD(int index) {
        switch (index) {
            case PIN_CLOCK:  return mPortCLKFD;
            case PIN_DATA:   return mPortDATFD;
            case PIN_BUTTON: return mPortBTNFD;
            default:
                 printf("[getPinFD] Unknown pin index %d, return FD_INVALID(%d)", index, -1);
                 return -1;
        }
    }

protected:
    void startListenThread();
    void stopListenThread();

private:
    static void* listenHandle(void* param);

private:
    int mPortCLK;
    int mPortDAT;
    int mPortBTN;
    int mPortCLKFD;
    int mPortDATFD;
    int mPortBTNFD;

    ThreadWrapper* mListenThread;
};

#endif
