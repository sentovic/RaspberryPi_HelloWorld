#include "system.h"
#include <iostream>

namespace cobox {

    uint64_t System::millsecond() {
        uint64_t tictoc = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
        return tictoc;
    }

}