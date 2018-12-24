#include "looper.h"
#include <iostream>
#include <memory.h>
#include <thread>
#include "message.h"
#include "system.h"
#include <algorithm>

namespace cobox {

    Looper::Looper() : 
                     mIsAlive(false), 
                     mIsReleased(false), 
                     mThread(Looper::guardRun, this) {
        
    }

    Looper::~Looper() {
        cleanMessageQueue();
    }

    void Looper::loop(bool isSync) {
        std::cout << "[Looper][loop]" << std::endl;

        mIsAlive = true;

        if (isSync) {
            mThread.join();
        } else {
            mThread.detach();
        }
    }

    void Looper::release() {
        mIsReleased = true;
    }

    void Looper::quit() {
        std::cout << "[Looper][quit]" << std::endl;
        mIsAlive = false;
    }

    bool Looper::queueMessage(Message* message, bool rejectIfNotAlive) {
        if (mIsReleased) {
            std::cerr << "[Looper][queueMessage] cannot queue message to a released looper" << std::endl;
            return false;
        }

        std::cout << "[Looper][queueMessage] what is " << message->what << std::endl;
        if ((message != NULL) && (rejectIfNotAlive && mIsAlive)) {
            mMessageQueueMutex.lock();
            try {
                mMessageQueue.push_back(message);
                sortMessageQueueByTime();
            } catch(...) {
                // TODO
            }
            std::cout << "[Looper][queueMessage] queued message what is " << message->what << std::endl;
            mMessageQueueEmptyCondition.notify_all();
            mMessageQueueMutex.unlock();

            return true;
        } else {
            return false;
        }
    }

    void Looper::sortMessageQueueByTime() {
        std::stable_sort(mMessageQueue.begin(), mMessageQueue.end(), [](Message* msgA, Message* msgB) {
            return msgA->mMessageTime < msgB->mMessageTime;
        });
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
            Message* message = nullptr;

            // Dequeue message
            {
                std::unique_lock<std::mutex> ulock(mMessageQueueMutex);
                try {
                    if (mMessageQueue.empty()) {
                        mMessageQueueEmptyCondition.wait(ulock, [this] {
                            return !this->mMessageQueue.empty();
                        });
                    }

                    message = mMessageQueue.front();
                    if (message != nullptr) {
                        // Check message timestamp
                        uint64_t now = System::millsecond();
                        uint64_t deltaTime = (message->mMessageTime - now);
                        if (deltaTime > 0) {
                            mMessageDelayCondition.wait_for(ulock, std::chrono::milliseconds(deltaTime));
                            continue;
                        }

                        mMessageQueue.pop_front();
                    }
                } catch (...) {
                    // TODO
                }
            }

            // Handle message
            if (message != NULL) {
                Handler* handler = message->getTarget();
                if (handler != NULL) {
                    handler->handleMessage(message);
                }
            }

        }
        std::cout << "[Looper][run] quit message queue loop" << std::endl;
    }

    void Looper::cleanMessageQueue() {
        mMessageQueue.clear();
    }
}