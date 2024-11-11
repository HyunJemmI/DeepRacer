#include "actuator/aa/servo_mgr.hpp"
#include <chrono>
#include <thread>

namespace PWM
{

    // Path to calibration file
    const std::string CAL_PATH = "/opt/aws/deepracer/calibration.json";

    // Location of the bios version on Ubuntu
    const std::string BIOS_PATH = "/sys/class/dmi/id/bios_version";

    // Keys for the calibration file
    const std::string HEADER_KEY = "Calibration";
    const std::string SERVO_KEY = "Servo";
    const std::string MOTOR_KEY = "Motor";
    const std::string MIN_KEY = "min";
    const std::string MID_KEY = "mid";
    const std::string MAX_KEY = "max";
    const std::string POLARITY_KEY = "polarity";
    const std::string VERSION_KEY = "version";

    // Enumeration for calibration type
    enum ServoCal
    {
        servo,
        motor
    };

    ServoMgr::ServoMgr()
        : throttle_(std::make_unique<Servo>(0)),
          angle_(std::make_unique<Servo>(1))
    {
        throttle_->setPeriod(SERVO_PERIOD);
        angle_->setPeriod(SERVO_PERIOD);

        calibrationMap_ = {{servo, {{MAX_KEY, SERVO_MAX}, {MID_KEY, SERVO_MID}, {MIN_KEY, SERVO_MIN}, {POLARITY_KEY, POLARITY_SERVO_VAL}}},
                           {motor, {{MAX_KEY, MOTOR_MAX}, {MID_KEY, MOTOR_MID}, {MIN_KEY, MOTOR_MIN}, {POLARITY_KEY, POLARITY_MOTOR_VAL}}}};
        if (checkFile(CAL_PATH))
        {
            setCalFromFile(calibrationMap_, CAL_PATH);
        }
        else
        {
            writeCalJSON(calibrationMap_, CAL_PATH);
        }
    }

    ServoMgr::~ServoMgr() = default;

    /// Subscriber for request made to the servo server.
    int ServoMgr::servoSubscriber(const float throttle, const float angle)
    {
        auto setPWM = [&](auto &servo, auto value, auto type)
        {
            if (value < -1.0 || value > 1.0)
            {
                return;
            }

            auto itCal = calibrationMap_.find(type);
            if (itCal == calibrationMap_.end())
            {
                return;
            }
            auto itMin = itCal->second.find(MIN_KEY);
            auto itMid = itCal->second.find(MID_KEY);
            auto itMax = itCal->second.find(MAX_KEY);
            auto itPolar = itCal->second.find(POLARITY_KEY);

            if (itMin != itCal->second.end() && itMid != itCal->second.end() && itMax != itCal->second.end() && itPolar != itCal->second.end())
            {
                float adjVal = value * itPolar->second;
                if (adjVal < 0)
                {
                    servo->setDuty(itMid->second + adjVal * (itMid->second - itMin->second));
                }
                else if (adjVal > 0)
                {
                    servo->setDuty(itMid->second + adjVal * (itMax->second - itMid->second));
                }
                else
                {
                    servo->setDuty(itMid->second);
                }
            }
        };

        setPWM(throttle_, throttle, motor);
        setPWM(angle_, angle, servo);
        // Make sure that the pulse goes through a full period
        std::this_thread::sleep_for(std::chrono::nanoseconds(SERVO_PERIOD));

        return 0;
    }

    /// Subscriber for setting the raw pwm topic.
    void ServoMgr::rawPWMSubscriber(const float throttle, const float angle)
    {
        if (throttle >= 0 && throttle <= SERVO_PERIOD)
        {
            throttle_->setDuty(throttle);
        }
        if (angle >= 0 && angle <= SERVO_PERIOD)
        {
            angle_->setDuty(angle);
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(SERVO_PERIOD));
    }

    /// Handler for setting the min and max value of the servos.
    int ServoMgr::setCalibrationValue(int cal_type, int min, int mid, int max, int polarity)
    {
        auto itCal = calibrationMap_.find(cal_type);

        if (itCal == calibrationMap_.end())
        {
            return 1;
        }

        auto itMin = itCal->second.find(MIN_KEY);
        auto itMid = itCal->second.find(MID_KEY);
        auto itMax = itCal->second.find(MAX_KEY);
        auto itPolar = itCal->second.find(POLARITY_KEY);

        if (itMin != itCal->second.end() && itMid != itCal->second.end() && itMax != itCal->second.end() && itPolar != itCal->second.end())
        {

            itMin->second = min;
            itMid->second = mid;
            itMax->second = max;
            itPolar->second = polarity;

            writeCalJSON(calibrationMap_, CAL_PATH);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    /// Handeler for seeting the servo GPIO pin to disable/enable.
    int ServoMgr::setGPIOActivation(bool enable)
    {
        bool ret = calibrateServo(GPIO_CHANNEL, enable);
        return (ret ? 0 : 1);
    }

    /// Handler for get calibration request
    int ServoMgr::getCalibrationValue(int cal_type, int *min, int *mid, int *max, int *polarity)
    {
        auto itCal = calibrationMap_.find(cal_type);

        if (itCal == calibrationMap_.end())
        {
            return 1;
        }

        auto itMin = itCal->second.find(MIN_KEY);
        auto itMid = itCal->second.find(MID_KEY);
        auto itMax = itCal->second.find(MAX_KEY);
        auto itPolar = itCal->second.find(POLARITY_KEY);

        if (itMin != itCal->second.end() && itMid != itCal->second.end() && itMax != itCal->second.end() && itPolar != itCal->second.end())
        {
            *min = itMin->second;
            *mid = itMid->second;
            *max = itMax->second;
            *polarity = itPolar->second;
            return 0;
        }
        else
        {
            return 1;
        }
    }

    /// Writes the given calibration map to file
    /// @param calibrationMap Calibration file to write to file
    /// @param filePath Full path to the desired file for which to write the given calibration map.
    int ServoMgr::writeCalJSON(const std::unordered_map<int, std::unordered_map<std::string, int>> &calibrationMap,
                               const std::string &filePath)
    {
        Json::Value calJsonValue;

        auto itServo = calibrationMap.find(servo);
        auto itMotor = calibrationMap.find(motor);

        if (itServo != calibrationMap.end() && itMotor != calibrationMap.end())
        {
            for (auto servoMap : itServo->second)
            {
                calJsonValue[HEADER_KEY][SERVO_KEY][servoMap.first] = servoMap.second;
            }
            for (auto motorMap : itMotor->second)
            {
                calJsonValue[HEADER_KEY][MOTOR_KEY][motorMap.first] = motorMap.second;
            }
            calJsonValue[HEADER_KEY][VERSION_KEY] = CAL_FILE_VERSION;
        }
        else
        {
            return 1;
        }
        writeJSONToFile(calJsonValue, filePath);
        return 0;
    }

    /// Reads the calibration from the desired file and stores the values in memory
    /// @param calibrationMap Map to store the calibrations read from file
    /// @param filePath Full file path of the file containing the calibrations, note the client verifies existence
    int ServoMgr::setCalFromFile(std::unordered_map<int, std::unordered_map<std::string, int>> &calibrationMap,
                                 const std::string &filePath)
    {
        Json::Value calJsonValue;
        Json::Reader reader;
        std::ifstream ifs(filePath);

        if (!reader.parse(ifs, calJsonValue))
        {
            return 1;
        }
        if (!calJsonValue.isMember("Calibration"))
        {
            return 1;
        }
        if (!calJsonValue[HEADER_KEY].isMember(SERVO_KEY) || !calJsonValue[HEADER_KEY].isMember(MOTOR_KEY))
        {
            return 1;
        }

        std::string biosVersion;
        std::ifstream bioFile(BIOS_PATH);

        if (bioFile.is_open())
        {
            std::getline(bioFile, biosVersion);
            bioFile.close();
        }

        if (BiosVersion(biosVersion) >= BiosVersion(BIOS_CUT_OFF))
        {
            if (!calJsonValue[HEADER_KEY].isMember(VERSION_KEY))
            {
                writeCalJSON(calibrationMap, filePath);
                return 1;
            }
        }

        auto populateMap = [&](auto &map, const auto &servoType)
        {
            for (auto &servoMap : map)
            {
                if (calJsonValue[HEADER_KEY][servoType].isMember(servoMap.first))
                {
                    servoMap.second = calJsonValue["Calibration"][servoType][servoMap.first].asInt();
                }
                else
                {
                    return false;
                }
            }
            return true;
        };

        auto tmpMap = calibrationMap;
        auto itServo = tmpMap.find(servo);
        auto itMotor = tmpMap.find(motor);

        if (itServo != tmpMap.end() && itMotor != tmpMap.end())
        {
            if (!populateMap(itServo->second, SERVO_KEY) || !populateMap(itMotor->second, MOTOR_KEY))
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
        calibrationMap = tmpMap;
        return 0;
    }

    /// Performs the calibration on the servo so that if the servo is turned on it can calibrate properly.
    /// @param pinNum GPIO pin number to enable or disable
    /// @param pinVal Enable/disable value for the servo, should be 0 or 1.
    bool ServoMgr::calibrateServo(int pinNum, int pinVal) const
    {
        if (pinVal > 1 || pinVal < 0 || pinNum < 0)
        {
            return false;
        }

        const int MAX_BUF = 64;
        auto writeGPIO = [&](const std::string &path, int pinNum,
                             const std::string &value)
        {
            char pinBuff[MAX_BUF];
            snprintf(pinBuff, sizeof(pinBuff), path.c_str(), pinNum);
            int fd = open(pinBuff, O_WRONLY);
            if (fd < 0)
            {
                return false;
            }
            char valueBuff[MAX_BUF];
            write(fd, valueBuff, snprintf(valueBuff, sizeof(valueBuff), "%s", value.c_str()));
            close(fd);
            return true;
        };
        bool ret = true;
        // Open the pin path and check its existence, if not create it.
        char buff[MAX_BUF];
        snprintf(buff, sizeof(buff), "/sys/class/gpio/gpio%d", pinNum);
        int gpioFd = open(buff, O_WRONLY);
        if (gpioFd < 0)
        {
            // Open the export path to export the pin.
            ret = ret && writeGPIO("/sys/class/gpio/export", pinNum, std::to_string(pinNum));
        }
        else
        {
            close(gpioFd);
        }
        // Write the direction, it is always "out"
        ret = ret && writeGPIO("/sys/class/gpio/gpio%d/direction", pinNum, "out");
        // Set the value, 0 for enable and 1 for disable.
        ret = ret && writeGPIO("/sys/class/gpio/gpio%d/value", pinNum, std::to_string(pinVal));

        memset(buff, 0, sizeof(buff));
        snprintf(buff, sizeof(buff), "/sys/class/gpio/gpio%d/value", pinNum);
        chmod(buff, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

        return ret;
    }
}
