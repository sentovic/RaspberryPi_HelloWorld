#include "application.h"
#include <iostream>

namespace cobox {

    Application::Application() : Looper() {
        mApplicationHandler = new Handler(this, std::bind(&Application::onHandleMessage, this, std::placeholders::_1));
    }

    Application::~Application() {
        delete mApplicationHandler;
    }

    void Application::loop() {
        std::cout << "[Application][loop]" << std::endl;
        Looper::loop(true);
    }

    void Application::run() {
        std::cout << "[Application][run]" << std::endl;
        mApplicationHandler->sendEmptyMessage(Application::MSG_APP_START);
        mApplicationHandler->sendEmptyMessageDelayed(Application::MSG_APP_STOP, 5000);
        Looper::run();
    }

    void Application::onHandleMessage(Message* message) {
        if (message == nullptr) {
            return;
        }
        
        switch (message->what) {
            case Application::MSG_APP_START: {
                std::cout << "[Application][onHandleMessage] MSG_APP_START" << std::endl;
                onCreated();
            } break;

            case Application::MSG_APP_STOP: {
                std::cout << "[Application][onHandleMessage] MSG_APP_STOP" << std::endl;
                onDestroy();
                quit();
            } break;

            default: break;
        }
    }

    void Application::exit() {
        std::cout << "[Application][exit]" << std::endl;
        mApplicationHandler->sendEmptyMessage(Application::MSG_APP_STOP);
    }

    void Application::onCreated() {
        std::cout << "[Application][onCreated]" << std::endl;
    }

    void Application::onDestroy() {
        std::cout << "[Application][onDestroy]" << std::endl;
    }


}