// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_ABORT_H_
#define ARA_CORE_ABORT_H_

namespace ara
{
namespace core
{

/// @uptrace{SWS_CORE_00053}
void AbortHandlerPrototype() noexcept;

/// @uptrace{SWS_CORE_00050}
using AbortHandler = decltype(&AbortHandlerPrototype);

/// @uptrace{SWS_CORE_00051}
AbortHandler SetAbortHandler(AbortHandler handler) noexcept;

/// @uptrace{SWS_CORE_00052}
[[noreturn]] void Abort(const char* text) noexcept;

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_ABORT_H_
