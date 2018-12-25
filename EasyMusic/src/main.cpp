#include <iostream>
#include "application.h"
#include "system.h"
#include "log.h"

int main() {
    cobox::Log::i("EasyMusic", "[EasyMusic] launch time is %ld", cobox::System::millsecond());

    cobox::Application application;
    application.loop();

    cobox::Log::i("EasyMusic", "[EasyMusic] quit");
    return 0;
}