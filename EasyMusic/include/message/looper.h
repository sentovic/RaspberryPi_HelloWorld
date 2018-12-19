#ifndef LOOPER_H
#define LOOPER_H

#include <stdbool.h>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <pthread.h>

namespace cobox {

    class Message;
    class Handler;
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

    private:
        pthread_t               mThread;
        bool                    mIsAlive;       
        std::queue<Message*>    mMessageQueue;
        std::mutex              mMessageQueueMutex;
        std::condition_variable mMessageQueueEmptyCondition;
    };

}

#endif