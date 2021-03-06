#include "application.h"
#include <iostream>

namespace cobox {

    Application::Application() : Looper() {
        mApplicationHandler = new Handler(this, (HandleMessageCallback*)onHandleMessage);
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
        Looper::run();
    }

    void Application::onHandleMessage(Message* message) {

    }

    void Application::onCreate() {}

    void Application::onDestroy() {}
}