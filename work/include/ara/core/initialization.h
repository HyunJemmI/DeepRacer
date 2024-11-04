// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_INITIALIZATION_H_
#define ARA_CORE_INITIALIZATION_H_

#include "ara/core/result.h"

namespace ara
{
namespace core
{

/// @uptrace{SWS_CORE_10001}
Result<void> Initialize();

/// @uptrace{SWS_CORE_10002}
Result<void> Deinitialize();

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_INITIALIZATION_H_
