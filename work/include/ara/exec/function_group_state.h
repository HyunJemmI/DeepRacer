// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_FUNCTION_GROUP_STATE_H_
#define ARA_EXEC_FUNCTION_GROUP_STATE_H_

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/exec/function_group.h"

namespace ara
{
namespace exec
{

/// @uptrace{SWS_EM_02269}
class FunctionGroupState final
{
  public:
    class CtorToken final
    {
      public:
        CtorToken(ara::core::StringView groupName, ara::core::StringView stateName) noexcept;
        ara::core::StringView GetGroup() const noexcept;
        ara::core::StringView GetState() const noexcept;
        bool operator==(CtorToken const& other) const noexcept;

      private:
        ara::core::String mGroup;
        ara::core::String mState;
    };

    /// @uptrace{SWS_EM_02270}
    static ara::core::Result<FunctionGroupState::CtorToken> Preconstruct(
        FunctionGroup const& functionGroup, ara::core::StringView metaModelIdentifier) noexcept;
    /// @uptrace{SWS_EM_02271}
    explicit FunctionGroupState(FunctionGroupState::CtorToken&& token) noexcept;
    /// @uptrace{SWS_EM_02272}
    virtual ~FunctionGroupState() noexcept;
    /// @uptrace{SWS_EM_02273}
    bool operator==(FunctionGroupState const& other) const noexcept;
    /// @uptrace{SWS_EM_02274}
    bool operator!=(FunctionGroupState const& other) const noexcept;

    /// @brief: vendor specific
    ara::core::StringView GetGroupName() const noexcept;
    ara::core::StringView GetStateName() const noexcept;

  private:
    CtorToken mToken;
};

}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_FUNCTION_GROUP_STATE_H_