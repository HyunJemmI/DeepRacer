// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_EXEC_FUNCTION_GROUP_H_
#define ARA_EXEC_FUNCTION_GROUP_H_

#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"

namespace ara
{
namespace exec
{

/// @uptrace{SWS_EM_02263}
class FunctionGroup final
{
  public:
    class CtorToken final
    {
      public:
        explicit CtorToken(ara::core::StringView name) noexcept;
        bool operator==(CtorToken const& other) const noexcept;
        ara::core::StringView GetName() const noexcept;

      private:
        ara::core::String mName;
    };

    /// @uptrace{SWS_EM_02264}
    static ara::core::Result<FunctionGroup::CtorToken> Preconstruct(ara::core::StringView metaModelIdentifier) noexcept;

    /// @uptrace{SWS_EM_02265}
    explicit FunctionGroup(FunctionGroup::CtorToken&& token) noexcept;
    /// @uptrace{SWS_ME_02266}
    virtual ~FunctionGroup() noexcept;

    /// @uptrace{SWS_EM_02267}
    bool operator==(FunctionGroup const& other) const noexcept;
    /// @uptrace{SWS_EM_02268}
    bool operator!=(FunctionGroup const& other) const noexcept;
    /// @brief: vendor specific
    ara::core::StringView GetName() const noexcept;

  private:
    CtorToken mToken;
};

}  // namespace exec
}  // namespace ara

#endif  // ARA_EXEC_FUNCTION_GROUP_H_