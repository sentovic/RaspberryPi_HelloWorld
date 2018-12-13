#include <iostream>
#include "application.h"

int main() {
    std::cout << "[EasyMusic]" << std::endl;

    cobox::Application application;
    application.loop();

    std::cout << "[EasyMusic]" << std::endl;
    return 0;
}