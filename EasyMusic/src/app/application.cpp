#include "application.h"
#include "log.h"

namespace cobox {

    Application::Application() : Looper() {
        mApplicationHandler = new Handler(this, std::bind(&Application::onHandleMessage, this, std::placeholders::_1));
    }

    Application::~Application() {
        delete mApplicationHandler;
    }

    void Application::loop() {
        Log::d("Application", "[loop]");
        Looper::loop(true);
    }

    void Application::run() {
        Log::d("Application", "[run]");
        mApplicationHandler->sendEmptyMessage(Application::MSG_APP_START);
        {// Debug
            mApplicationHandler->sendEmptyMessage(Application::MSG_APP_STOP);
        }
        Looper::run();
    }

    void Application::onHandleMessage(Message* message) {
        if (message == nullptr) {
            return;
        }
        
        switch (message->what) {
            case Application::MSG_APP_START: {
                Log::i("Application", "[onHandleMessage] MSG_APP_START");
                onCreated();
            } break;

            case Application::MSG_APP_STOP: {
                Log::i("Application", "[onHandleMessage] MSG_APP_STOP");
                onDestroy();
                quit();
            } break;

            case Application::MSG_APP_CONFIG_CHANGE: {
                Log::i("Application", "[onHandleMessage] MSG_APP_CONFIG_CHANGE");
            } break;

            default: break;
        }
    }

    void Application::exit() {
        Log::i("Application", "[exit]");
        mApplicationHandler->sendEmptyMessage(Application::MSG_APP_STOP);
    }

    void Application::onCreated() {
        Log::i("Application", "[onCreated]");
    }

    void Application::onDestroy() {
        Log::i("Application", "[onDestroy]");
    }


}