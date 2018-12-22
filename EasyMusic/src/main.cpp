#include <iostream>
#include "application.h"
#include "system.h"

int main() {
    std::cout << "[EasyMusic]" << std::endl;
    std::cout << "[EasyMusic] launch time is " << cobox::System::millsecond() << std::endl;

    cobox::Application application;
    application.loop();

    std::cout << "[EasyMusic]" << std::endl;
    return 0;
}