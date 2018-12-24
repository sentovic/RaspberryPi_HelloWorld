#include "system.h"

namespace cobox {

    uint64_t System::millsecond() {
        return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    }

}