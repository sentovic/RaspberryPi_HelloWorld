#include "system.h"

namespace cobox {

    uint64_t System::millsecond() {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

}