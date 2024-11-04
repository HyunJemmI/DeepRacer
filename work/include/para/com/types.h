// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_COM_TYPES_H
#define PARA_COM_TYPES_H

#include <cstdint>
#include <sstream>
#include <string>

namespace para
{
namespace com
{

using ServiceId = std::uint16_t;
using InstanceId = std::uint16_t;
using MajorVersion = std::uint8_t;
using MinorVersion = std::uint32_t;

using MethodToken = std::uint64_t;

struct VersionScheme
{
    MajorVersion major;
    MinorVersion minor;

    inline bool operator==(const VersionScheme& rhs) const
    {
        return major == rhs.major && minor == rhs.minor;
    }

    inline bool operator<(const VersionScheme& rhs) const
    {
        if (major < rhs.major) {
            return true;
        }
        if ((major == rhs.major) && (minor < rhs.minor)) {
            return true;
        }
        return false;
    }
};

struct ServiceHandle
{
    ServiceId serviceId;
    InstanceId instanceId;
    VersionScheme version;

    inline bool operator==(const ServiceHandle& rhs) const
    {
        return serviceId == rhs.serviceId && instanceId == rhs.instanceId && version == rhs.version;
    }

    inline std::string ToString() const
    {
        std::stringstream ss;
        ss << serviceId << ':' << instanceId << ':' << unsigned(version.major) << ':' << version.minor;
        return ss.str();  // FIXME: cache the string
    }
};

}  // namespace com
}  // namespace para

template <>
struct std::hash<para::com::ServiceHandle>
{
    std::size_t operator()(para::com::ServiceHandle const& obj) const noexcept
    {
        std::stringstream ss;
        ss << obj.serviceId << ':' << obj.instanceId << ':' << obj.version.major << ':' << obj.version.minor;
        return std::hash<std::string>{}(ss.str());
    }
};

#endif  // PARA_COM_TYPES_H
