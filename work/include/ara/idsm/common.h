// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_IDSM_COMMON_H_
#define ARA_IDSM_COMMON_H_

#include <cstdint>

#include "ara/core/span.h"

namespace ara
{
namespace idsm
{

/// @brief ContextDataType used for sending context data to the IdsM.
/// @uptrace{SWS_AIDSM_10201}
using ContextDataType = ara::core::Span<std::uint8_t>;

/// @brief TimestampType used for setting optional sensor-specific timestamp for events.
/// @uptrace{SWS_AIDSM_10202}
using TimestampType = std::uint64_t;

/// @brief CountType used for setting optional count for events pre-qualified by sensors.
/// @uptrace{SWS_AIDSM_10203}
using CountType = std::uint16_t;

}  // namespace idsm
}  // namespace ara

#endif  // ARA_IDSM_COMMON_H_