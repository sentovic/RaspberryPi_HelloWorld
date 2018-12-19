#ifndef APPLICATION_H
#define APPLICATION_H

#include "looper.h"
#include "handler.h"
#include "message.h"

namespace cobox {

    class Application : public Looper {
    public:
        Application();
        ~Application();

        void loop();

    protected:
        void run();
        virtual void onCreate();
        virtual void onDestroy();

    private:
        void onHandleMessage(Message* message);

    protected:
        static const int MSG_APP_START = 0x1000;
        static const int MSG_APP_STOP  = 0x1001;

    private:
        Handler* mApplicationHandler;
    };

}

#endif