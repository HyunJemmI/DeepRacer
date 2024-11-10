#ifndef BIOS_VERSION_HPP
#define BIOS_VERSION_HPP

#include <string>

namespace PWM {
    /// This is the hardware rev where we need to start checking
    /// calibration file because of a manufacturing polarity issue
    const std::string BIOS_CUT_OFF = "0.0.8";
    class BiosVersion
    {
    /// Helper class for comparing bios versions
    public:
        BiosVersion(const std::string &biosVersion) {
            int ret = std::sscanf(biosVersion.c_str(), "%d.%d.%d", &first_, &second_, &third_);
            if (ret <=0) {
                std::sscanf(BIOS_CUT_OFF.c_str(), "%d.%d.%d", &first_, &second_, &third_);
            }
        }
        bool operator >= (const BiosVersion &rhs) const {
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
    private:
        /// Bios version is comprised of three digits first.second.thirds
        /// version comparaon starts with the first digit
        int first_;
        int second_;
        int third_;
    };
}
#endif