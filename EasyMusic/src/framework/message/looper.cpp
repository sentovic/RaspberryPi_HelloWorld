#include "looper.h"
#include "log.h"
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
        Log::d("Looper", "[loop]");

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
        Log::i("Looper", "[quit]");
        mIsAlive = false;
    }

    bool Looper::queueMessage(Message* message, bool rejectIfNotAlive) {
        if (mIsReleased) {
            Log::e("Looper", "[queueMessage] cannot queue message to a released looper");
            return false;
        }

        Log::d("Looper", "[queueMessage] what is %d", message->what);
        if ((message != NULL) && (rejectIfNotAlive && mIsAlive)) {
            mMessageQueueMutex.lock();
            try {
                mMessageQueue.push_back(message);
                sortMessageQueueByTime();
            } catch(...) {
                // TODO
            }

            Log::d("Looper", "[queueMessage] queued message what is %d", message->what);
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
        Log::d("Looper", "[guardRun]");
        Looper* looper = static_cast<Looper*>(rawLooper);
        if (looper != NULL) {
            looper->run();
            looper->release();
        }
        return NULL;
    }

    void Looper::run() {
        Log::d("Looper", "[run]");
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
                        int64_t deltaTime = (message->mMessageTime - now);
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
                // Invoke callback first
                std::function<void()> callback = message->mCallback;
                if (callback != nullptr) {
                    callback();
                } else {
                    // If no callback in message, send it to target handler
                    Handler* handler = message->getTarget();
                    if (handler != NULL) {
                        handler->handleMessage(message);
                    }
                }
            }

        }
        
        Log::i("Looper", "[run] quit message queue loop");
    }

    void Looper::cleanMessageQueue() {
        mMessageQueue.clear();
    }
}