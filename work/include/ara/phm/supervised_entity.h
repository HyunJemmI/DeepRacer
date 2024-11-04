// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PHM_SUPERVISED_ENTITY_H_
#define ARA_PHM_SUPERVISED_ENTITY_H_

#include <cstdint>
#include <type_traits>

#include "ara/core/instance_specifier.h"
#include "ara/phm/internal/supervised_entity_impl.h"
#include "ara/phm/phm_error_domain.h"

namespace ara
{
namespace phm
{

/// @brief Enumeration of local supervision status. Scoped Enumeration of uint32_t.
/// @uptrace{SWS_PHM_01136}
enum class LocalSupervisionStatus : std::uint32_t
{
    kDeactivated = 0,  // Supervision is not active.
    kOK = 1,           // Supervision is active and no failure is present
    kFailed = 2,       // A failure was detected but still within tolerance of debouncing
    kExpired = 3       // A failure was detected and qualified
};

/// @brief Enumeration of global supervision status. Scoped Enumeration of uint32_t
/// @uptrace{SWS_PHM_01137}
enum class GlobalSupervisionStatus : std::uint32_t
{
    kDeactivated = 0,  // Supervision is not active.
    kOK = 1,           // All local supervisions are in status kOK or kDeactivated.
    kFailed = 2,       // At least one local supervision is in status kFailed but none in status kExpired
    kExpired = 3,      // At least one local supervision is in status kExpired but the number of Supervision Cycle since
                       // reaching kExpired has not exceeded the tolerance.
    kStopped = 4       // At least one local supervision is in status kExpired and the number of Supervision Cycle since
                       // reaching kExpired has exceeded the tolerance.
};

/// @brief SupervisedEntity Class.
/// @param EnumT An enum type that contains supervised entity identifier
/// @uptrace{SWS_PHM_01132}
template <typename EnumT>
class SupervisedEntity
{
  public:
    /// @brief Creation of a SupervisedEntity.
    /// @param instance  instance specifier of the supervised entity
    /// @uptrace{SWS_PHM_01123}
    explicit SupervisedEntity(const ara::core::InstanceSpecifier& instance)
        : mImpl(std::make_unique<SupervisedEntityImpl>(instance))
    {}

    /// @brief Destructor of a SupervisedEntity
    /// @uptrace{SWS_PHM_01211}
    ~SupervisedEntity() noexcept = default;

    /// @uptrace{SWS_PHM_01214}
    SupervisedEntity(SupervisedEntity&& se) noexcept = default;

    /// @brief Move constructor for SupervisedEntity.
    /// @param se  The SupervisedEntity object to be moved.
    /// @uptrace{SWS_PHM_01215}
    SupervisedEntity& operator=(SupervisedEntity&& se) noexcept = default;

    /// @brief The copy constructor for SupervisedEntity shall not be used.
    /// @uptrace{SWS_PHM_01212}
    SupervisedEntity(const SupervisedEntity& se) = delete;

    /// @brief The copy assignment operator for SupervisedEntity shall not be used.
    /// @uptrace{SWS_PHM_01213}
    SupervisedEntity& operator=(const SupervisedEntity& se) = delete;

    /// @brief Reports an occurrence of a Checkpoint.
    /// @param checkpointId  checkpoint identifier.
    /// @return A Result, being either empty or containing an implementation specific error code.
    /// @uptrace{SWS_PHM_01127}
    ara::core::Result<void> ReportCheckpoint(EnumT checkpointId)
    {
        if (mImpl == nullptr) {
            // return error - supervised entity is stopped
            return ara::core::Result<void>(ara::phm::PhmErrc::kWrongInstance);
        }
        if (!std::is_enum<EnumT>::value) {
            // return error - checkpoint id is not enum
            return ara::core::Result<void>(ara::phm::PhmErrc::kCheckpointNotAvailable);
        }
        // send checkpoint - checkpoint id to phm
        return mImpl->ReportCheckpoint(static_cast<std::uint32_t>(checkpointId));
    }

    /// @brief Returns the local supervision status that the supervised entity belongs to.
    /// @return A Result, being either the local supervision status or containing an implementation specific error code.
    /// @uptrace{SWS_PHM_01134}
    ara::core::Result<LocalSupervisionStatus> GetLocalSupervisionStatus() const
    {
        if (mImpl == nullptr) {
            // return error - supervised entity is stopped
            return ara::core::Result<LocalSupervisionStatus>(ara::phm::PhmErrc::kWrongInstance);
        }
        return mImpl->GetLocalSupervisionStatus();
    }

    /// @brief Returns the global supervision status that the supervised entity belongs to
    /// @return A Result, being either the global supervision status or containing an implementation specific error
    /// code.
    /// @uptrace{SWS_PHM_01135}
    ara::core::Result<GlobalSupervisionStatus> GetGlobalSupervisionStatus() const
    {
        // request global supervision status from phm
        if (mImpl == nullptr) {
            // return error - supervised entity is stopped
            return ara::core::Result<GlobalSupervisionStatus>(ara::phm::PhmErrc::kWrongInstance);
        }
        return mImpl->GetGlobalSupervisionStatus();
    }

  private:
    using IntegerType = typename std::underlying_type<EnumT>::type;
    // The implementation class
    std::unique_ptr<SupervisedEntityImpl> mImpl;
};

}  // namespace phm
}  // namespace ara

#endif  // ARA_PHM_SUPERVISED_ENTITY_H_