// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_UTILITY_CONVERT_H_
#define PARA_UTILITY_CONVERT_H_

#include <array>
#include <string>
#include <vector>

#include "ara/log/common.h"
#include "para/types/type_alias.h"

namespace para
{
namespace utility
{

std::string GetUUID();
std::string GetParaRoot();
std::string GetParaLogLevel();
std::string GetParaProcessName();
std::string GetParaProcessCategory();
std::string GetProcessNameByPid(pid_t pid);
std::vector<std::string> Tokenizer(const std::string& data, char delimiter);
double Random(double low, double high);
std::string ReplaceAll(const std::string& str, const std::string& pattern, const std::string& replace);

std::string GetTimeStr(time_t time);

std::vector<Byte> IntToByteVec(int value, int size, unsigned char defaultVal);

template <std::size_t N>
std::int32_t ByteArrayToInt(const std::array<Byte, N>& array)
{
    std::string str(std::begin(array), std::end(array));
    return strtol(str.c_str(), nullptr, 10);
}

std::uint64_t htonll(std::uint64_t value);
std::uint64_t ntohll(std::uint64_t value);

ara::log::LogLevel ConvertStringToLogLevel(const std::string& input);

}  // namespace utility
}  // namespace para

#if 0
char* CONV_GetHexaValue(char* value, std::int32_t length, char* buff, std::int32_t buffSize);
void CONV_PrintHexa(char* values, std::int32_t length, std::int32_t logLevel);

// @brief: type converter
void CONV_ByteVecToString1(std::vector<Byte>& src, std::string& dest);
std::string CONV_ByteVecToString2(std::vector<Byte>& src);
std::vector<Byte> CONV_IntToByteVec(std::int32_t value, std::int32_t size, unsigned char defaultVal);
template<std::size_t N>
std::string CONV_ByteArrayToString(const std::array<Byte, N>& array)
{
    std::string str(std::begin(array), std::end(array));
    return str;
}
template<std::size_t N>
std::array<Byte, N> CONV_IntToByteArray(int value, unsigned char defaultVal)
{
    char buf[256] = {0, };
    std::array<Byte, N> result(N, defaultVal);
    snprintf(buf, sizeof(buf), "%d", value);
    for (int i = 0; i < (int)strlen(buf); i++) {
        result[i] = buf[i];
    }

    return result;
}
#endif
#endif  // PARA_UTILITY_CONVERT_H_
