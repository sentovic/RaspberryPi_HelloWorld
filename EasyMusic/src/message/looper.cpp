#include "looper.h"
#include <iostream>
#include <memory.h>
#include <pthread.h>
#include "message.h"

namespace cobox {

    Looper::Looper() : mIsAlive(false) {
        
    }

    Looper::~Looper() {
        cleanMessageQueue();
    }

    void Looper::loop(bool isSync) {
        std::cout << "[Looper][loop]" << std::endl;;
    
        if (mIsAlive = (pthread_create(&mThread, NULL, guardRun, this) == 0)) {
            // TODO
        } else {
            std::cerr << "[" __FILE__ "] create looper thread failed!" << std::endl;
            memset(&mThread, 0, sizeof(pthread_t));
            return;
        }

        if (isSync) {
            pthread_join(mThread, NULL);
        }
    }

    void Looper::release() {
        pthread_exit(NULL);
    }

    void Looper::quit() {
        std::cout << "[Looper][quit]" << std::endl;
        mIsAlive = false;
    }

    bool Looper::queueMessage(Message* message, bool rejectIfNotAlive) {
        std::cout << "[Looper][queueMessage] what is " << message->what << std::endl;
        if ((message != NULL) && (rejectIfNotAlive && mIsAlive)) {
            std::unique_lock<std::mutex> lock(mMessageQueueMutex);
            lock.lock();
            {
                mMessageQueue.push(message);
            }
            lock.unlock();

            std::cout << "[Looper][queueMessage] queued message what is " << message->what << std::endl;
            mMessageQueueEmptyCondition.notify_all();
            return true;
        } else {
            return false;
        }
    }

    void* Looper::guardRun(void* rawLooper) {
        std::cout << "[Looper][guardRun]" << std::endl;
        Looper* looper = static_cast<Looper*>(rawLooper);
        if (looper != NULL) {
            looper->run();
            looper->release();
        }
        return NULL;
    }

    void Looper::run() {
        std::cout << "[Looper][run]" << std::endl;
        while (mIsAlive) {
            std::cout << "[Looper][run] #1" << std::endl;
            std::unique_lock<std::mutex> lock(mMessageQueueMutex);
            if (mMessageQueue.empty()) {
                std::cout << "[Looper][run] #1.1" << std::endl;
                mMessageQueueEmptyCondition.wait(lock, [this] {
                    return !this->mMessageQueue.empty();
                });
                std::cout << "[Looper][run] #1.2" << std::endl;
            }

            std::cout << "[Looper][run] #2" << std::endl;
            lock.lock();
            {
                std::cout << "[Looper][run] #2.1" << std::endl;
                Message* message = mMessageQueue.front();
                if (message != NULL) {
                    std::cout << "[Looper][run] #2.2" << std::endl;
                    mMessageQueue.pop();

                    Handler* handler = message->getTarget();
                    if (handler != NULL) {
                        std::cout << "[Looper][run] #2.3" << std::endl;
                        handler->handleMessage(message);
                    }
                }
            }
            lock.unlock();
            std::cout << "[Looper][run] #3" << std::endl;

            std::cout << "[Looper][run] mIsAlive = " << mIsAlive << std::endl;
        }
        std::cout << "[Looper][run] quit message queue loop" << std::endl;
    }

    void Looper::cleanMessageQueue() {
        std::queue<Message*> empty;
        swap(empty, mMessageQueue);
    }
}