#include "led_mgr.hpp"

namespace PWM {
    // LED defines
    #define LED_OFF 0
    #define LED_ON 10000000

    // Path to LED values file
    const std::string LED_VAL_PATH = "/opt/aws/deepracer/led_values.json";

    // Keys for the LED values file
    const std::string LED_HEADER_KEY = "Led Values";
    const std::string RED_CHANNEL_KEY = "red_pwm";
    const std::string GREEN_CHANNEL_KEY = "green_pwm";
    const std::string BLUE_CHANNEL_KEY = "blue_pwm";

    enum LEDColors
    {
        red,
        green,
        blue,
        totalColor
    };
   
    LedMgr::LedMgr()
    {
        ledVec_ = { (std::make_shared<Servo>(2)), (std::make_shared<Servo>(3)), (std::make_shared<Servo>(4))};
        ledValuesMap_ = {{{RED_CHANNEL_KEY, LED_OFF},{GREEN_CHANNEL_KEY, LED_OFF},
                                    {BLUE_CHANNEL_KEY, LED_ON}}}; // Default

        if (checkFile(LED_VAL_PATH)) {
            setLedValFromFile(ledValuesMap_, LED_VAL_PATH);
        }
        else {
            writeLedValJSON(ledValuesMap_, LED_VAL_PATH);
        }

        std::string colorVec[3] = {RED_CHANNEL_KEY, GREEN_CHANNEL_KEY, BLUE_CHANNEL_KEY};
        for (size_t color=0; color<ledVec_.size(); ++color) {
            ledVec_[color]->setPeriod(SERVO_PERIOD);
            ledVec_[color]->setDuty(ledValuesMap_[colorVec[color]]);
        }
    }

    LedMgr::~LedMgr()
    {
        for(auto color : ledVec_) {
            color->setDuty(LED_OFF);
        }
    }

    /// Pointer to the red, green and blue led pwm
    /// handler for the led requests
    int LedMgr::setLedValue(int red_value, int green_value, int blue_value) {
        if (red_value < 0 || red_value > 10000000 ||
                green_value < 0 || green_value > 10000000 ||
                blue_value < 0 || blue_value > 10000000) {
            return 1;
        }

        ledVec_[red]->setDuty(red_value);
        ledVec_[green]->setDuty(green_value);
        ledVec_[blue]->setDuty(blue_value);

        // Write the updated values in the file
        ledValuesMap_[RED_CHANNEL_KEY] = red_value;
        ledValuesMap_[GREEN_CHANNEL_KEY] = green_value;
        ledValuesMap_[BLUE_CHANNEL_KEY] = blue_value;

        writeLedValJSON(ledValuesMap_, LED_VAL_PATH);

        return 0;
    }

    /// Handler for get led values request
    int LedMgr::getLedValue(int *red_value, int *green_value, int *blue_value) {
        if (ledValuesMap_.find(RED_CHANNEL_KEY) == ledValuesMap_.end() || ledValuesMap_.find(GREEN_CHANNEL_KEY) == ledValuesMap_.end() || ledValuesMap_.find(BLUE_CHANNEL_KEY) == ledValuesMap_.end()) {
            return 1;
        }
        *red_value = ledValuesMap_[RED_CHANNEL_KEY];
        *green_value = ledValuesMap_[GREEN_CHANNEL_KEY];
        *blue_value = ledValuesMap_[BLUE_CHANNEL_KEY];
        return 0;
    }

    /// Reads the LED values from the desired file and stores the values in memory
    /// @param ledValuesMap Map to store the calibrations read from file
    /// @param filePath Full file path of the file containing the calibrations, note the client verifies existence
    int LedMgr::setLedValFromFile(std::unordered_map<std::string, int> &ledValuesMap, const std::string &filePath) {
        Json::Value ledJsonValue;
        Json::Reader reader;
        std::ifstream ifs(filePath);

        if (!reader.parse(ifs, ledJsonValue)) {
            return 1;
        }
        if (!ledJsonValue.isMember(LED_HEADER_KEY)) {
            return 1;
        }
        if (!ledJsonValue[LED_HEADER_KEY].isMember(RED_CHANNEL_KEY) || !ledJsonValue[LED_HEADER_KEY].isMember(GREEN_CHANNEL_KEY) || !ledJsonValue[LED_HEADER_KEY].isMember(BLUE_CHANNEL_KEY)) {
            return 1;
        }
        ledValuesMap[RED_CHANNEL_KEY] = ledJsonValue[LED_HEADER_KEY][RED_CHANNEL_KEY].asInt();
        ledValuesMap[GREEN_CHANNEL_KEY] = ledJsonValue[LED_HEADER_KEY][GREEN_CHANNEL_KEY].asInt();
        ledValuesMap[BLUE_CHANNEL_KEY] = ledJsonValue[LED_HEADER_KEY][BLUE_CHANNEL_KEY].asInt();

        return 0;
    }

    /// Writes the given led values map to file
    /// @param ledValuesMap Led values to write to file
    /// @param filePath Full path to the desired file for which to write the given led values map.
    int LedMgr::writeLedValJSON(std::unordered_map<std::string, int> &ledValuesMap, const std::string &filePath) {
        Json::Value ledValuesJsonValue;

        if (ledValuesMap.find(RED_CHANNEL_KEY) != ledValuesMap.end() && ledValuesMap.find(GREEN_CHANNEL_KEY) != ledValuesMap.end() && ledValuesMap.find(BLUE_CHANNEL_KEY) != ledValuesMap.end()) {
            ledValuesJsonValue[LED_HEADER_KEY][RED_CHANNEL_KEY] = ledValuesMap[RED_CHANNEL_KEY];
            ledValuesJsonValue[LED_HEADER_KEY][GREEN_CHANNEL_KEY] = ledValuesMap[GREEN_CHANNEL_KEY];
            ledValuesJsonValue[LED_HEADER_KEY][BLUE_CHANNEL_KEY] = ledValuesMap[BLUE_CHANNEL_KEY];
        }
        else {
            return 1;
        }
        writeJSONToFile(ledValuesJsonValue, filePath);

        return 0;
    }
}