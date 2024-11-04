// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_TYPES_H_
#define ARA_COM_TYPES_H_

#include <cstdint>

#include "ara/com/com_error_domain.h"
#include "ara/com/sample_ptr.h"
#include "ara/core/instance_specifier.h"
#include "ara/core/vector.h"

namespace ara
{
namespace com
{

/// @uptrace{SWS_CM_00301}
enum class MethodCallProcessingMode : std::uint8_t
{
    kPoll,
    kEvent,
    kEventSingleThread
};

/// @uptrace{SWS_CM_00310}
enum class SubscriptionState : std::uint8_t
{
    kSubscribed,
    kNotSubscribed,
    kSubscriptionPending
};

/// @uptrace{SWS_CM_00308}
template <typename T>
using SampleAllocateePtr = std::unique_ptr<T>;

/// @uptrace{SWS_CM_00309}
using EventReceiveHandler = std::function<void()>;

/// @uptrace{SWS_CM_00311}
using SubscriptionStateChangeHandler = std::function<void(ara::com::SubscriptionState)>;

/// @uptrace{SWS_CM_00304}
template <typename T>
using ServiceHandleContainer = ara::core::Vector<T>;

/// @uptrace{SWS_CM_00303}
struct FindServiceHandle
{
    FindServiceHandle() = default;

    FindServiceHandle(ara::core::InstanceSpecifier specifier, ara::core::String serviceId, ara::core::String instanceId)
        : instanceSpecifier(std::move(specifier)), serviceId(std::move(serviceId)), instanceId(std::move(instanceId))
    {}

    virtual ~FindServiceHandle() = default;
    FindServiceHandle(const FindServiceHandle&) = default;
    FindServiceHandle& operator=(const FindServiceHandle&) = default;
    FindServiceHandle(FindServiceHandle&&) noexcept = default;
    FindServiceHandle& operator=(FindServiceHandle&&) noexcept = default;

    bool operator==(const FindServiceHandle& other) const
    {
        return ((instanceSpecifier == other.instanceSpecifier) && (serviceId == other.serviceId) &&
                (instanceId == other.instanceId));
    }

    bool operator<(const FindServiceHandle& other) const
    {
        return (std::stoull(instanceId.c_str()) < std::stoull(other.instanceId.c_str()));
    }

    ara::core::InstanceSpecifier instanceSpecifier{"undefined"};
    ara::core::String serviceId{"undefined"};
    ara::core::String instanceId{"undefined"};
};

/// @uptrace{SWS_CM_00383}
template <typename T>
using FindServiceHandler = std::function<void(ServiceHandleContainer<T>, FindServiceHandle)>;

}  // namespace com
}  // namespace ara

#endif  // ARA_COM_TYPES_H_
