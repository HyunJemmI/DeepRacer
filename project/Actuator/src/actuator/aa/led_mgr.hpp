#ifndef LED_MGR_HPP
#define LED_MGR_HPP

#include "pwm.hpp"
#include "utility.hpp"
#include <unordered_map>

namespace PWM {
    #define SERVO_PERIOD 20000000
    class LedMgr
        {
        public:
            LedMgr();
            ~LedMgr();
            /// Pointer to the red, green and blue led pwm
            /// handler for the led requests
            int setLedValue(int red_value, int green_value, int blue_value);
                           

            /// Handler for get led values request
            int getLedValue(int *red_value, int *green_value, int *blue_value);
 
        private:
            /// Reads the LED values from the desired file and stores the values in memory
            /// @param ledValuesMap Map to store the calibrations read from file
            /// @param filePath Full file path of the file containing the calibrations, note the client verifies existence
            int setLedValFromFile(std::unordered_map<std::string, int> &ledValuesMap, const std::string &filePath);

            /// Writes the given led values map to file
            /// @param ledValuesMap Led values to write to file
            /// @param filePath Full path to the desired file for which to write the given led values map.
            int writeLedValJSON(std::unordered_map<std::string, int> &ledValuesMap, const std::string &filePath);

            std::vector<std::shared_ptr< Servo >> ledVec_;
            /// Hash map that stores the pwm values of each color channel.
            std::unordered_map<std::string, int> ledValuesMap_;
    	};
}
#endif