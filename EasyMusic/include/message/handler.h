#ifndef HANDLER_H
#define HANDLER_H

#include "lang.h"
#include "looper.h"

namespace cobox {
    class Message;

    typedef void(*HandleMessageCallback)(Message*);
    
    class Handler {
        public:
            Handler(Looper* looper);
            Handler(Looper* looper, HandleMessageCallback callback);
            ~Handler();
        
            void sendMessage(Message* message);
            void sendEmptyMessage(int what);
            void removeAllMeesagesAndCallbacks();
            void post(Runnable* runnable);

        protected:
            friend class Looper;
            void handleMessage(Message* message);
            virtual void onHandleMessage(Message* message);

        private:
            HandleMessageCallback mHandleMessageCallback;
            Looper* mLooper;
    };
}

#endif