#include "actuator/aa/bios_version.hpp"


namespace PWM {
    BiosVersion::BiosVersion(const std::string &biosVersion)
    {
        int ret = std::sscanf(biosVersion.c_str(), "%d.%d.%d", &first_, &second_, &third_);
        if (ret <=0) {
            std::sscanf(BIOS_CUT_OFF.c_str(), "%d.%d.%d", &first_, &second_, &third_);
        }
    }
    bool BiosVersion::operator >= (const BiosVersion &rhs) {
        if (first_ > rhs.first_) {
            return true;
        }
        if (second_ > rhs.second_) {
            return true;
        }
        if (third_ > rhs.third_) {
            return true;
        }
        if (first_ == rhs.first_
            && second_ == rhs.second_ &&
            third_ == rhs.third_) {
            return true;
        }
        return false;
    }
}