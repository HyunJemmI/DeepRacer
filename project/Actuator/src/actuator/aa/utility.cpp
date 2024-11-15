#include "utility.hpp"

namespace PWM {
    /// Check's if the desired file exist
    /// @returns True if file exist, false otherwise
    /// @param filePath Full path to desired file for which to validate existence
    bool checkFile(const std::string &filePath) {
        int fd = open(filePath.c_str(), O_RDONLY);
        close(fd);
        return fd >= 0;
    }

    /// Writes the given calibration map to file
    /// @param jsonValue json value to write to file
    /// @param filePath Full path to the desired file for which to write the given json.
    void writeJSONToFile(Json::Value jsonValue, const std::string &filePath) {
        // Write to file
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "   ";

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ofstream ofs(filePath);
        writer->write(jsonValue, &ofs);
    }
}
