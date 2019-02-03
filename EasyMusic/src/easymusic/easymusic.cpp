#include "easymusic.h"
#include "log.h"
#include "curl/curl.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include <string>
#include <stdio.h>

typedef size_t(*ON_RESPONSE_WRITE)(char*, size_t, size_t, std::string*);

const std::string MUSIC_API::API_SONG   ("http://music.163.com/song/media/outer/url?id=%s.mp3");
const std::string MUSIC_API::API_ARTIST ("http://music.163.com/artist?id=%s");
const std::string MUSIC_API::API_ALBUM  ("http://music.163.com/album?id=%s");

const std::string WEATHER_API::API_APP_KEY          ("b823ec79fc85b850559aa75155915db2");
const std::string WEATHER_API::API_CITY_CODE        ("1795563"); // Shenzhen
const std::string WEATHER_API::API_REALTIME_WEATHER ("http://api.openweathermap.org/data/2.5/weather?appid=%s&id=%s");

EasyMusic::EasyMusic() {
    mNetworkHandler = new cobox::Handler(this, [this] (cobox::Message *msg) {
        switch (msg->what) {
            case MSG_UPDATE_WEATHER: {
                this->updateWeather();
                this->mNetworkHandler->sendEmptyMessageDelayed(MSG_UPDATE_WEATHER, 1000 * 60 * 10); // 10min
            } break;
        }
    });
}

EasyMusic::~EasyMusic() {
    delete mNetworkHandler;
    mNetworkHandler = nullptr;
}

void EasyMusic::onCreated() {
    cobox::Application::onCreated();
    cobox::Log::i("EasyMusic", "[onCreated]");

    mNetworkHandler->sendEmptyMessage(MSG_UPDATE_WEATHER);

    
}

void EasyMusic::onDestroy() {
    cobox::Application::onDestroy();
    cobox::Log::i("EasyMusic", "[onDestroy]");
}

void EasyMusic::updateWeather() {
    CURL *curl;
    CURLcode result;
    std::string buffer;
 
    curl = curl_easy_init();
    if (curl != nullptr) {
        char url[512] = {0};
        sprintf(url, WEATHER_API::API_REALTIME_WEATHER.c_str(), WEATHER_API::API_APP_KEY.c_str(), WEATHER_API::API_CITY_CODE.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
            static_cast<ON_RESPONSE_WRITE> ([](char* data, size_t count, size_t unitSize, std::string* out) -> size_t {
                std::string text((const char *)data, count * unitSize);
                ((std::string *)out)->append(text);
                return count * unitSize;
            }));
 
        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            cobox::Log::e("EasyMusic", "[updateWeather] curl_easy_perform() failed: %s", curl_easy_strerror(result));
        } else {
            long responseCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            cobox::Log::i("EasyMusic", "[onCreated] Response: %d", responseCode);
            cobox::Log::i("EasyMusic", "[onCreated] Content: %s", buffer.c_str());


        }
        curl_easy_cleanup(curl);
    }   
}