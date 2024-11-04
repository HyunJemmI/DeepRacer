// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_RECOVERY_ACTION_H_
#define ARA_PHM_RECOVERY_ACTION_H_

#include "ara/core/instance_specifier.h"
#include "ara/phm/internal/recovery_action_impl.h"
#include "ara/phm/phm_error_domain.h"
#include "ara/phm/supervised_entity.h"

namespace ara
{
namespace phm
{

/// @uptrace{SWS_PHM_01138}
enum class TypeOfSupervision : std::uint32_t
{
    AliveSupervision = 0,
    DeadlineSupervision = 1,
    LogicalSupervision = 2
};

/// @uptrace{SWS_PHM_01140}
class RecoveryAction
{
  public:
    /// @uptrace{SWS_PHM_01141}
    explicit RecoveryAction(const ara::core::InstanceSpecifier& instance)
        : mImpl(std::make_unique<RecoveryActionImpl>(instance))
    {
        auto handler = [this](const std::string& functionGroup, const std::uint32_t& executionError,
                              std::uint32_t supervision) -> void {
            ara::core::StringView fgStr(functionGroup.data());
            ara::core::Result<ara::exec::FunctionGroup::CtorToken> fgResult =
                ara::exec::FunctionGroup::Preconstruct(fgStr);
            ara::exec::FunctionGroup::CtorToken fgToken(fgResult.ValueOrThrow());
            ara::exec::FunctionGroup fg(std::move(fgToken));
            RecoveryHandler(ara::exec::ExecutionErrorEvent{executionError, fg},
                            static_cast<TypeOfSupervision>(supervision));
        };
        mImpl->SetActionHandler(handler);
    }

    /// @uptrace{SWS_PHM_01145}
    ~RecoveryAction() noexcept = default;

    /// @uptrace{SWS_PHM_01149}
    RecoveryAction(RecoveryAction&& ra) noexcept
    {
        // move ctor
        mImpl = std::move(ra.mImpl);
        auto handler = [this](const std::string& functionGroup, const std::uint32_t& executionError,
                              std::uint32_t supervision) -> void {
            ara::core::StringView fgStr(functionGroup.data());
            ara::core::Result<ara::exec::FunctionGroup::CtorToken> fgResult =
                ara::exec::FunctionGroup::Preconstruct(fgStr);
            ara::exec::FunctionGroup::CtorToken fgToken(fgResult.ValueOrThrow());
            ara::exec::FunctionGroup fg(std::move(fgToken));
            RecoveryHandler(ara::exec::ExecutionErrorEvent{executionError, fg},
                            static_cast<TypeOfSupervision>(supervision));
        };
        mImpl->SetActionHandler(handler);
        ra.mImpl = nullptr;
    }

    /// @uptrace{SWS_PHM_01151}
    RecoveryAction& operator=(RecoveryAction&& ra) noexcept
    {
        // move operator
        mImpl = std::move(ra.mImpl);
        auto handler = [this](const std::string& functionGroup, const std::uint32_t& executionError,
                              std::uint32_t supervision) -> void {
            ara::core::StringView fgStr(functionGroup.data());
            ara::core::Result<ara::exec::FunctionGroup::CtorToken> fgResult =
                ara::exec::FunctionGroup::Preconstruct(fgStr);
            ara::exec::FunctionGroup::CtorToken fgToken(fgResult.ValueOrThrow());
            ara::exec::FunctionGroup fg(std::move(fgToken));
            RecoveryHandler(ara::exec::ExecutionErrorEvent{executionError, fg},
                            static_cast<TypeOfSupervision>(supervision));
        };
        mImpl->SetActionHandler(handler);
        ra.mImpl = nullptr;
        return *this;
    }

    /// @uptrace{SWS_PHM_01150}
    RecoveryAction(const RecoveryAction& ra) = delete;
    /// @uptrace{SWS_PHM_01152}
    RecoveryAction& operator=(const RecoveryAction& ra) = delete;

    /// @uptrace{SWS_PHM_01143}
    ara::core::Result<void> Offer()
    {
        return mImpl->Offer();
    }

    /// @uptrace{SWS_PHM_01143}
    void StopOffer()
    {
        // disconnect with phm
        mImpl->StopOffer();
    }

    /// @uptrace{SWS_PHM_01146}
    ara::core::Result<GlobalSupervisionStatus> GetGlobalSupervisionStatus() const
    {
        return mImpl->GetGlobalSupervisionStatus();
    }

    /// @uptrace{SWS_PHM_01142}
    virtual void RecoveryHandler(const ara::exec::ExecutionErrorEvent& executionError,
                                 TypeOfSupervision supervision) = 0;

  private:
    // The implementation class
    std::unique_ptr<RecoveryActionImpl> mImpl;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_RECOVERY_ACTION_H_