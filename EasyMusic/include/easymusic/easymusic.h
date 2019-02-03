#pragma once

#include "application.h"
#include "handler.h"
#include <string>

#ifndef API_CONST
#define API_CONST
    struct MUSIC_API {
        static const std::string API_SONG;
        static const std::string API_ARTIST;
        static const std::string API_ALBUM;
    };

    struct WEATHER_API {
        static const std::string API_APP_KEY;
        static const std::string API_CITY_CODE;
        static const std::string API_REALTIME_WEATHER;
    };
#endif

class EasyMusic : public cobox::Application {
    public:
        EasyMusic();
        ~EasyMusic();

        void onCreated();
        void onDestroy();

    private:
        static const int MSG_UPDATE_WEATHER = 0;

    private:
        void updateWeather();

    private:
        cobox::Handler *mNetworkHandler;
};