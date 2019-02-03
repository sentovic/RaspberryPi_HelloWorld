#pragma once

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
            void post(std::function<void()> runnable);
            void postDelayed(std::function<void()> runnable, uint64_t delay);

        protected:
            friend class Looper;
            void handleMessage(Message* message);

        private:
            Looper* mLooper;
            std::function<void (Message*)> mHandleMessageCallback;
    };
}
