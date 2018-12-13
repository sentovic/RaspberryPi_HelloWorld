#ifndef HANDLER_H
#define HANDLER_H

#include "lang.h"
#include "looper.h"

namespace cobox {

    class Message;
    class Handler {
        public:
            Handler(Looper* looper);
            ~Handler();
        
            void sendMessage(Message* message);
            void sendEmptyMessage(int what);
            void removeAllMeesagesAndCallbacks();
            void post(Runnable* runnable);

        protected:
            friend class Looper;
            void handleMessage(Message* message);

        private:
            Looper* mLooper;
    };
}

#endif