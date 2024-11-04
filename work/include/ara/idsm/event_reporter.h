// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_IDSM_EVENT_REPORTER_H_
#define ARA_IDSM_EVENT_REPORTER_H_

#include <memory>

#include "ara/core/instance_specifier.h"
#include "ara/idsm/common.h"

namespace ara
{
namespace idsm
{

/// @brief Class for reporting security events to the IdsM.
/// @uptrace{SWS_AIDSM_10101}
class EventReporter
{
  public:
    /// @brief Construct a new Event Reporter object.
    /// @uptrace{SWS_AIDSM_10301}
    EventReporter(const ara::core::InstanceSpecifier& eventType);

    virtual ~EventReporter();

    EventReporter(EventReporter&) = delete;

    EventReporter(EventReporter&&) = default;

    EventReporter& operator=(EventReporter&) = delete;

    EventReporter& operator=(EventReporter&&) = default;

    /// @brief Create a new security event at the IdsM
    /// @uptrace{SWS_AIDSM_10302}
    void ReportEvent(const CountType = 1) noexcept;

    /// @brief Create a new security event with a sensor-provided timestamp at the IdsM
    /// @uptrace{SWS_AIDSM_10303}
    void ReportEvent(const TimestampType timestamp, const CountType = 1) noexcept;

    /// @brief Create a new security event with a sensor-provided context data at the IdsM
    /// @uptrace{SWS_AIDSM_10304}
    void ReportEvent(const ContextDataType& contextData, const CountType = 1) noexcept;

    /// @brief Create a new security event with a sensor-provided context data and timestamp at the IdsM
    /// @uptrace{SWS_AIDSM_10305}
    void ReportEvent(const ContextDataType& contextData, const TimestampType timestamp, const CountType = 1) noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};

}  // namespace idsm
}  // namespace ara

#endif  // ARA_IDSM_EVENT_REPORTER_H_