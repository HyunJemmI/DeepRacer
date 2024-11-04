// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_EXECUTION_CLIENT_H_
#define ARA_EXEC_EXECUTION_CLIENT_H_

#include <map>
#include <memory>
#include <string>

#include "ara/core/result.h"

namespace ara
{
namespace exec
{

/// @uptrace{SWS_EM_02000}
enum class ExecutionState : std::uint8_t
{
    kRunning = 0,
};
const std::map<ExecutionState, std::string> kExecutionStateStr = {
    {ExecutionState::kRunning, "Running"},
};

/// @uptrace{SWS_EM_02070}
enum class ExecutionReturnType : std::uint8_t
{
    kSuccess = 0,
    kGeneralError = 1
};
const std::map<ExecutionReturnType, std::string> kExecutionReturnTypeStr = {
    {ExecutionReturnType::kSuccess, "succ"},
    {ExecutionReturnType::kGeneralError, "error"},
};

/// @uptrace{SWS_EM_02001}
class ExecutionClient
{
  public:
    /// @uptrace{SWS_EM_02030}
    ExecutionClient() noexcept;
    /// @uptrace{SWS_EM_02002}
    virtual ~ExecutionClient() noexcept;

    /// @uptrace{SWS_EM_02003}
    ara::core::Result<void> ReportExecutionState(ExecutionState state) const noexcept;

  private:
    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;
};

}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_EXECUTION_CLIENT_H_