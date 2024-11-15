#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <memory>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "json/json.h"

namespace PWM {
        
    /// Check's if the desired file exist
    /// @returns True if file exist, false otherwise
    /// @param filePath Full path to desired file for which to validate existence
    bool checkFile(const std::string &filePath);

    /// Writes the given calibration map to file
    /// @param jsonValue json value to write to file
    /// @param filePath Full path to the desired file for which to write the given json.
    void writeJSONToFile(Json::Value jsonValue, const std::string &filePath);

}
#endif
