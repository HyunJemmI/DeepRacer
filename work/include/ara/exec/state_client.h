// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_STATE_CLIENT_H_
#define ARA_EXEC_STATE_CLIENT_H_

#include <memory>
#include <string>

#include "ara/core/future.h"
#include "ara/core/promise.h"
#include "ara/exec/exec_error_domain.h"
#include "ara/exec/execution_error_event.h"
#include "ara/exec/function_group.h"
#include "ara/exec/function_group_state.h"

namespace ara
{
namespace exec
{

/// @uptrace{SWS_EM_02275}
class StateClient
{
  public:
    /// @uptrace{SWS_EM_02276}
    explicit StateClient(std::function<void(FunctionGroup&)> undefinedStateCallback) noexcept;
    /// @uptrace{SWS_EM_02277}
    virtual ~StateClient() noexcept;

    /// @uptrace{SWS_EM_02278}
    ara::core::Future<void> SetState(FunctionGroupState const& state) const noexcept;
    /// @uptrace{SWS_EM_02279}
    ara::core::Future<void> GetInitialMachineStateTransitionResult() const noexcept;
    /// @uptrace{SWS_EM_02542}
    ara::core::Result<ExecutionErrorEvent> GetExecutionError(FunctionGroup const& functionGroup) noexcept;
    // Get FunctionGroupState
    ara::core::Result<ara::core::String> GetState(ara::core::String functionGroup) noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};


}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_STATE_CLIENT_H_