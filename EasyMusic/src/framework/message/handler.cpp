#include "handler.h"
#include <stddef.h>
#include "log.h"
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
        Log::d("Handler", "[sendMessage] what is %d", message->what);

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
        Log::d("Handler", "[sendEmptyMessage] what is %d", what);
        Message* msg = new Message();
        msg->mTarget      = this;
        msg->mMessageTime = System::millsecond() + delay;
        msg->what         = what;
        if (mLooper != nullptr) {
            mLooper->queueMessage(msg);
        }
    }

    void Handler::removeAllMeesagesAndCallbacks() {
        Log::d("Handler", "[removeAllMeesagesAndCallbacks]");
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
        Log::d("Handler", "[handleMessage] message is " + message->toString());

        if (mHandleMessageCallback != nullptr) {
            mHandleMessageCallback(message);
        }
    }

}