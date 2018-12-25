#include "message.h"
#include <stddef.h>

namespace cobox {

    Message::Message() : 
                       mTarget(nullptr), 
                       what(0),
                       arg1(0),
                       arg2(0),
                       anything(nullptr),
                       mCallback(nullptr),
                       mMessageTime(TIME_IGNORE) {

    }

    Message::~Message() {
        mTarget = nullptr;
        what = 0;
        arg1 = 0;
        arg2 = 0;
        mCallback = nullptr;
    }

    void Message::bindTarget(Handler* target) {
        mTarget = target;
    }

    Handler* Message::getTarget() {
        return mTarget;
    }

    void Message::sendToTarget() {
        if (mTarget != nullptr) {
            mTarget->sendMessage(this);
        }
    }

    std::string Message::toString() {
        std::string content = "{ when=" + std::to_string(this->mMessageTime)
                  + ", what=" + std::to_string(this->what)
                  + ", target=" + std::to_string((long) this->mTarget)
                  + ", callback=" + std::to_string((long) &(this->mCallback))
                  + " }";
        return content;
    }
}