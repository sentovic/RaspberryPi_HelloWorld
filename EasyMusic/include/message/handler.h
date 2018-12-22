#pragma once

#include "lang.h"
#include "looper.h"
#include <functional>

namespace cobox {

    class Message;

    class Handler {
        public:
            Handler(Looper* looper, std::function<void (Message*)> callback = nullptr);
            ~Handler();
        
            void sendMessage(Message* message);
            void sendMessageDelayed(Message* message, uint64_t delay);
            void sendEmptyMessage(int what);
            void sendEmptyMessageDelayed(int what, uint64_t delay);
            void removeAllMeesagesAndCallbacks();
            void post(Runnable runnable);
            void postDelayed(Runnable runnable, uint64_t delay);

        protected:
            friend class Looper;
            void handleMessage(Message* message);

        private:
            Looper* mLooper;
            std::function<void (Message*)> mHandleMessageCallback;
    };
}
