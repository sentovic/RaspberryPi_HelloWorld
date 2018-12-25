#include "handler.h"
#include <stddef.h>
#include <iostream>
#include "message.h"
#include "system.h"

namespace cobox {

    Handler::Handler(Looper* looper, std::function<void (Message*)> callback) :
                                     mLooper(looper),
                                     mHandleMessageCallback(callback) {

    }

    Handler::~Handler() {
        mLooper = NULL;
    }

    void Handler::sendMessage(Message* message) {
        sendMessageDelayed(message, 0);
    }

    void Handler::sendMessageDelayed(Message* message, uint64_t delay) {
        if (message == nullptr) {
            return;
        }
        std::cout << "[Handler][sendMessage] what is " << message->what << std::endl;

        message->mMessageTime = System::millsecond() + delay;
        message->mTarget      = this;
        if (mLooper != nullptr) {
            mLooper->queueMessage(message);
        }
    }

    void Handler::sendEmptyMessage(int what) {
        sendEmptyMessageDelayed(what, 0);
    }

    void Handler::sendEmptyMessageDelayed(int what, uint64_t delay) {
        std::cout << "[Handler][sendEmptyMessage] what is " << what << std::endl;
        Message* msg = new Message();
        msg->mTarget      = this;
        msg->mMessageTime = System::millsecond() + delay;
        msg->what         = what;
        if (mLooper != nullptr) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::removeAllMeesagesAndCallbacks() {
        std::cout << "[Handler][removeAllMeesagesAndCallbacks]" << std::endl;
        if (mLooper != nullptr) {
            mLooper->cleanMessageQueue();
        }
    }

    void Handler::post(std::function<void()> runnable) {
        postDelayed(runnable, 0);
    }

    void Handler::postDelayed(std::function<void()> runnable, uint64_t delay) {
        if (runnable == nullptr) {
            return;
        }

        Message* msg = new Message();
        msg->mTarget      = this;
        msg->mMessageTime = System::millsecond() + delay;
        msg->mCallback    = runnable;
        if (mLooper != nullptr) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::handleMessage(Message* message) {
        std::cout << "[Handler][handleMessage] message is ";
        message->toString(std::cout);
        std::cout << std::endl;

        if (mHandleMessageCallback != nullptr) {
            mHandleMessageCallback(message);
        }
    }

}