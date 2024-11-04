// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_STEADY_CLOCK_H_
#define ARA_CORE_STEADY_CLOCK_H_

#include <chrono>
#include <cstdint>

namespace ara
{
namespace core
{

/// @brief This clock represents a monotonic clock.
///        The time points of this clock cannot decrease as physical time moves forward and the time between ticks of
///        this clock is constant.
/// @uptrace{SWS_CORE_06401}
/// @uptrace{SWS_CORE_11800}
class SteadyClock final
{
  public:
    /// @uptrace{SWS_CORE_06412}
    using rep = std::int64_t;
    /// @uptrace{SWS_CORE_06413}
    using period = std::nano;
    /// @uptrace{SWS_CORE_06411}
    using duration = std::chrono::duration<rep, period>;
    /// @uptrace{SWS_CORE_06414}
    using time_point = std::chrono::time_point<SteadyClock, duration>;
    /// @uptrace{SWS_CORE_06431}
    static constexpr bool is_steady = true;

    /// @uptrace{SWS_CORE_06432}
    /// @uptrace{SWS_CORE_11801}
    static time_point now() noexcept
    {
        duration dur(std::chrono::steady_clock::now().time_since_epoch());
        time_point tp(dur);
        return tp;
    }
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_STEADY_CLOCK_H_
