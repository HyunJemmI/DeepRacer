// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_EXECUTION_ERROR_EVENT_H_
#define ARA_EXEC_EXECUTION_ERROR_EVENT_H_

#include "ara/exec/function_group.h"

namespace ara
{
namespace exec
{
/// @uptrace{SWS_EM_02541}
using ExecutionError = std::uint32_t;

/// @uptrace{SWS_EM_02544}
struct ExecutionErrorEvent final
{
    /// @uptrace{SWS_EM_02545}
    ExecutionError executionError;
    /// @uptrace{SWS_EM_02546}
    FunctionGroup functionGroup;
};

}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_EXECUTION_ERROR_EVENT_H_
