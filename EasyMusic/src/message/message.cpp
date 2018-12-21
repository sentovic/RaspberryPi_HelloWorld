#include "message.h"
#include <stddef.h>

namespace cobox {

    Message::Message() : 
                       mTarget(NULL), 
                       what(0),
                       arg1(0),
                       arg2(0),
                       anything(NULL),
                       mCallback(NULL),
                       mMessageTime(TIME_IGNORE) {

    }

    Message::~Message() {
        mTarget = NULL;
        what = 0;
        arg1 = 0;
        arg2 = 0;
        mCallback = NULL;
    }

    void Message::bindTarget(Handler* target) {
        mTarget = target;
    }

    Handler* Message::getTarget() {
        return mTarget;
    }

    void Message::sendToTarget() {
        if (mTarget != NULL) {
            mTarget->sendMessage(this);
        }
    }

    void Message::toString(std::ostream& out) {
        out << "Message[what=" << this->what << ", callback=" << this->mCallback << "]";
    }
}