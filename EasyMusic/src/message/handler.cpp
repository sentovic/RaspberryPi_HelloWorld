#include "handler.h"
#include <stddef.h>
#include <iostream>
#include "message.h"

namespace cobox {

    Handler::Handler(Looper* looper) :
                                     mLooper(looper),
                                     mHandleMessageCallback(NULL) {

    }

    Handler::Handler(Looper* looper, HandleMessageCallback callback) : 
                                                                      mLooper(looper),
                                                                      mHandleMessageCallback(callback) {

    }

    Handler::~Handler() {
        mLooper = NULL;
    }

    void Handler::sendMessage(Message* message) {
        if (message == NULL) {
            return;
        }

        std::cout << "[Handler][sendMessage] what is " << message->what << std::endl;
        message->mTarget = this;
        if (mLooper != NULL) {
            mLooper->queueMessage(message);
        }
    }

    void Handler::sendEmptyMessage(int what) {
        std::cout << "[Handler][sendEmptyMessage] what is " << what << std::endl;
        Message* msg = new Message();
        msg->mTarget = this;
        msg->what = what;
        if (mLooper != NULL) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::removeAllMeesagesAndCallbacks() {
        if (mLooper != NULL) {
            mLooper->cleanMessageQueue();
        }
    }

    void Handler::post(Runnable* runnable) {
        if (runnable == NULL) {
            return;
        }

        Message* msg = new Message();
        msg->mTarget = this;
        msg->mCallback = runnable;
        if (mLooper != NULL) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::handleMessage(Message* message) {
        std::cout << "[Handler][handleMessage] message is ";
        message->toString(std::cout);
        std::cout << std::endl;

        if (mHandleMessageCallback != NULL) {
            mHandleMessageCallback(message);
        }

        delete message;
        message = NULL;
    }

    void Handler::onHandleMessage(Message* message) {}

}