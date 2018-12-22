#pragma once

#include <stdbool.h>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace cobox {

    class Message;
    class Looper {
    public:
        Looper();
        ~Looper();

        void loop(bool isSync = false);
        void quit();
        bool queueMessage(Message* message, bool rejectIfNotAlive = true);

    protected:
    friend class Handler;
        virtual void run();
        void release();
        void cleanMessageQueue();

    private:
        static void* guardRun(void* looper);
        void sortMessageQueueByTime();

    private:
        std::thread             mThread;
        bool                    mIsAlive;
        bool                    mIsReleased;
        std::queue<Message*>    mMessageQueue;
        std::mutex              mMessageQueueMutex;
        std::condition_variable mMessageQueueEmptyCondition;
    };

}