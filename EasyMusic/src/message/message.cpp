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

    void Message::toString(std::ostream& out) {
        out << "{ when=" << this->mMessageTime
            << ", what=" << this->what
            << ", target=" << this->mTarget
            << ", callback=" << &(this->mCallback)
            << " }";
    }
}