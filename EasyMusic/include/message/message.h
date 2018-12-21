#ifndef MESSAGE_H
#define MESSAGE_H

#include "lang.h"
#include "handler.h"
#include <iostream>

namespace cobox {

    class Message {
        protected:
        friend class Handler;
        friend class Looper;
            Message();
            ~Message();
            void     bindTarget(Handler* target);
            Handler* getTarget();

        public:
            void     sendToTarget();
            void     toString(std::ostream& out);

        public:
            int      what;
            int      arg1;
            int      arg2;
            void*    anything;
            
        private:
            Handler* mTarget;
            Runnable mCallback;
            long     mMessageTime;

        private:
            static const long TIME_IGNORE = -1;
    };
}

#endif