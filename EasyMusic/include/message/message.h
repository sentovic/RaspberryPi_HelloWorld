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
            void     toString(std::ostream& out);

        public:
            void     sendToTarget();

        public:
            int      what;
            int      arg1;
            int      arg2;
            void*    anything;
            
        private:
            Handler* mTarget;
            Runnable* mCallback;
    };
}

#endif