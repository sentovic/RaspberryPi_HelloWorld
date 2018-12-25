#pragma once

#include "looper.h"
#include "handler.h"
#include "message.h"

namespace cobox {

    class Application : public Looper {
    public:
        Application();
        ~Application();

        void loop();
        void exit();

    protected:
        void run();
        virtual void onCreated();
        virtual void onDestroy();
        virtual void onHandleMessage(Message* message);

    protected:
        static const int MSG_APP_START         = 0x1000;
        static const int MSG_APP_STOP          = 0x1001;
        static const int MSG_APP_CONFIG_CHANGE = 0x1002;

    private:
        Handler* mApplicationHandler;
    };

}
