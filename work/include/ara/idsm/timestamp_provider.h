// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_IDSM_TIMESTAMP_PROVIDER_H_
#define ARA_IDSM_TIMESTAMP_PROVIDER_H_

#include <functional>

#include "ara/idsm/common.h"

namespace ara
{
namespace idsm
{

/// @brief Register a callback for providing timestamps to the IdsM.
/// @uptrace{SWS_AIDSM_20101}
void RegisterTimestampProvider(std::function<TimestampType()> callback);

}  // namespace idsm
}  // namespace ara

#endif  // ARA_IDSM_TIMESTAMP_PROVIDER_H_