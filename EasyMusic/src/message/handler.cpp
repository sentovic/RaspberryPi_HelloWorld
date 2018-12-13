#include "handler.h"
#include <stddef.h>
#include <iostream>
#include "message.h"

namespace cobox {

    Handler::Handler(Looper* looper) :
                                     mLooper(looper) {

    }

    Handler::~Handler() {
        mLooper = NULL;
    }

    void Handler::sendMessage(Message* message) {
        // TODO
    }

    void Handler::sendEmptyMessage(int what) {
        // TODO
        std::cout << "[Handler][sendEmptyMessage] what is " << what << std::endl;
        Message* msg = new Message();
        msg->what = what;
        if (mLooper != NULL) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::removeAllMeesagesAndCallbacks() {
        // TODO
    }

    void Handler::post(Runnable* runnable) {
        // TODO
        Message* msg = new Message();
        
    }

    void Handler::handleMessage(Message* message) {
        std::cout << "[Handler][handleMessage] message is ";
        message->toString(std::cout);
        std::cout << std::endl;
    }

}