#ifndef SERVO_MGR_HPP
#define SERVO_MGR_HPP

#include <sys/stat.h>
#include "pwm.hpp"
#include "bios_version.hpp"
#include "utility.hpp"
#include <unordered_map>

namespace PWM {
    /// Current version of the calibration file, when incremented
    /// check writeJSONToFile and setCalFromFile to ensure backward
    /// compatibility
    #define CAL_FILE_VERSION 1
    /// Default servo parameters provided by pegatron. All values in ns
    #define SERVO_MAX 1700000
    #define SERVO_MID 1450000
    #define SERVO_MIN 1200000
    #define MOTOR_MAX 1603500
    #define MOTOR_MID 1446000
    #define MOTOR_MIN 1311000
    #define POLARITY_SERVO_VAL 1
    #define POLARITY_MOTOR_VAL -1
    #define SERVO_PERIOD 20000000
    /// Servo GPIO channel, set by Pega
    #define GPIO_CHANNEL 436

    class ServoMgr
        {
        public:
            ServoMgr();
            ~ServoMgr();
            int servoSubscriber(const float throttle, const float angle);
            void rawPWMSubscriber(const float throttle, const float angle);
            
            /// Handler for setting the min and max value of the servos.
            int setCalibrationValue(int cal_type, int min, int mid, int max, int polarity);

            /// Handler for seeting the servo GPIO pin to disable/enable.
            int setGPIOActivation(bool enable);
               
            /// Handler for get calibration request
            int getCalibrationValue(int cal_type, int* min, int *mid, int *max, int *polarity);


        private:
            /// Writes the given calibration map to file
            /// @param calibrationMap Calibration file to write to file
            /// @param filePath Full path to the desired file for which to write the given calibration map.
            int writeCalJSON(const std::unordered_map<int, std::unordered_map<std::string, int>> &calibrationMap, const std::string &filePath);

            /// Reads the calibration from the desired file and stores the values in memory
            /// @param calibrationMap Map to store the calibrations read from file
            /// @param filePath Full file path of the file containing the calibrations, note the client verifies existence
            int setCalFromFile(std::unordered_map<int, std::unordered_map<std::string, int>> &calibrationMap, const std::string &filePath);

            /// Performs the calibration on the servo so that if the servo is turned on it can calibrate properly.
            /// @param pinNum GPIO pin number to enable or disable
            /// @param pinVal Enable/disable value for the servo, should be 0 or 1.
            bool calibrateServo(int pinNum, int pinVal) const ;

            /// Pointer to the throttle servo
            std::unique_ptr<Servo> throttle_;
            /// Ponter to the steering servo.
            std::unique_ptr<Servo> angle_;
            /// Hash map that stores the min/max of each servo.
            std::unordered_map<int, std::unordered_map<std::string, int>> calibrationMap_;
    };
}
#endif