// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_UTILITY_CONFIG_H_
#define PARA_UTILITY_CONFIG_H_

#include <cstdint>
#include <string>
#include <vector>

namespace para
{
namespace utility
{

std::int32_t GetIntConf(std::string path, std::string section, std::string key, std::int32_t* value);
std::int32_t GetStringConf(std::string path, std::string section, std::string key, std::string* value);
std::int32_t GetStringConf(std::string path, std::string section, std::string key, std::vector<std::string>* vector);

}  // namespace utility
}  // namespace para

#endif  // PARA_UTILITY_CONFIG_H_
