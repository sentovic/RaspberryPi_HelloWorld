#pragma once

#include "handler.h"
#include <iostream>
#include <functional>

namespace cobox {

    class Message {
        friend class Handler;
        friend class Looper;
        
        protected:
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
            Handler*              mTarget;
            std::function<void()> mCallback;
            uint64_t              mMessageTime;

        private:
            static const uint64_t TIME_IGNORE = 0;
    };
}
