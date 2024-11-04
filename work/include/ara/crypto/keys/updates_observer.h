// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H
#define ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H

#include <memory>

#include "ara/crypto/keys/elementary_types.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Definition of an "updates observer" interface.
///        The "updates observer" interface should be implemented by a consumer application,
///        if a software developer would like to get notifications about the slots’ content update events.
/// @uptrace{SWS_CRYPT_30200}
class UpdatesObserver
{
  public:
    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_30201}
    using Uptr = std::unique_ptr<UpdatesObserver>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_30210}
    virtual ~UpdatesObserver() noexcept = default;

    /// @brief Notification method that should be called if content of specified slots was changed.
    ///        Key Storage engine should call this method in a dedicated thread.
    ///        The provided list may include only slots subscribed for observing (during openning with the "User"
    ///        permissions, i.e. for "reading" via a call of the method OpenAsUser()). Each slot number may present in
    ///        the provided list only one time!
    /// @param updatedSlots List of monitored slots that were updated after opening (for reading)
    /// @uptrace{SWS_CRYPT_30211}
    virtual void OnUpdate(const TransactionScope& updatedSlots) noexcept = 0;

    /// @brief Copy-assign another UpdatesObserver to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30224}
    UpdatesObserver& operator=(const UpdatesObserver& other) = default;

    /// @brief Move-assign another UpdatesObserver to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30225}
    UpdatesObserver& operator=(UpdatesObserver&& other) = default;
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H