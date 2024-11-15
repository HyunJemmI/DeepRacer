#ifndef BIOS_VERSION_HPP
#define BIOS_VERSION_HPP

#include <string>

namespace PWM {
 /// This is the hardware rev where we need to start checking
 /// calibration file because of a manufacturing polarity issue
 const std::string BIOS_CUT_OFF = "0.0.8";
 class BiosVersion {
    /// Helper class for comparing bios versions
    public:
        BiosVersion(const std::string &biosVersion);
        bool operator >= (const BiosVersion &rhs);
    private:
        /// Bios version is comprised of three digits first.second.thirds
        /// version comparaon starts with the first digit
        int first_;
        int second_;
        int third_;
    };
}
#endif