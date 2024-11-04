// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H
#define ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H

#include <memory>

#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/crypto/keys/keyslot.h"
#include "ara/crypto/keys/updates_observer.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Key Storage Provider interface. Any object is uniquely identified by the combination of its UUID and type.
///        HSMs/TPMs implementing the concept of "non-extractable keys" should use own copies of externally supplied
///        crypto objects. A few software Crypto Providers can share single key slot if they support same format.
/// @uptrace{SWS_CRYPT_30100}
class KeyStorageProvider
{
  public:
    /// @brief --
    /// @uptrace{SWS_CRYPT_30101}
    using Uptr = std::unique_ptr<KeyStorageProvider>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_30110}
    virtual ~KeyStorageProvider() noexcept = default;

    /// @brief Begin new transaction for key slots update.
    ///        In order for a keyslot to be part of a transaction scope, the reserveSpareSlot model parameter of the
    ///        keyslot has to be set to true. A transaction is dedicated for updating related key slots simultaneously
    ///        (in an atomic, all-or-nothing, way). All key slots that should be updated by the transaction have to be
    ///        opened and provided to this function. Any changes to the slots in scope are executed by calling commit().
    /// @param targetSlots a list of KeySlots that should be updated during this transaction.
    /// @return a unique ID assigned to this transaction
    /// @uptrace{SWS_CRYPT_30123}
    virtual ara::core::Result<TransactionId> BeginTransaction(const TransactionScope& targetSlots) noexcept = 0;

    /// @brief Commit changes of the transaction to Key Storage.
    ///        Any changes of key slots made during a transaction are invisible up to the commit execution.
    ///        The commit command permanently saves all changes made during the transaction in Key Storage.
    /// @param id an ID of a transaction that should be commited
    /// @return --
    /// @uptrace{SWS_CRYPT_30124}
    virtual ara::core::Result<void> CommitTransaction(TransactionId id) noexcept = 0;

    /// @brief Get pointer of registered Updates Observer. The method returns nullptr if no observers have been
    /// registered yet!
    /// @return unique pointer to the registered Updates Observer interface
    ///         (copy of an internal unique pointer is returned, i.e. the Key Storage provider continues to keep the
    ///         ownership)
    /// @uptrace{SWS_CRYPT_30131}
    virtual UpdatesObserver::Uptr GetRegisteredObserver() const noexcept = 0;

    /// @brief Load a key slot. The functions loads the information associated with a KeySlot into a KeySlot object.
    /// @param iSpecify the target key-slot instance specifier
    /// @return an unique smart pointer to allocated key slot
    /// @uptrace{SWS_CRYPT_30115}
    virtual ara::core::Result<KeySlot::Uptr> LoadKeySlot(ara::core::InstanceSpecifier& iSpecify) noexcept = 0;

    /// @brief Register consumer Updates Observer.
    ///        Only one instance of the UpdatesObserver may be registered by an application process,
    ///        therefore this method always unregister previous observer and return its unique pointer.
    ///        If (nullptr == observer) then the method only unregister the previous observer!
    ///        The method returns nullptr if no observers have been registered yet!
    /// @param observer optional pointer to a client-supplied Updates Observer instance that should be registered inside
    ///        Key Storage implementation and called every time, when an opened for usage/loading key slot is updated
    ///        externally (by its "Owner" application)
    /// @return unique pointer to previously registered Updates Observer interface (the pointer ownership is "moved out"
    /// to the caller code)
    /// @uptrace{SWS_CRYPT_30130}
    virtual UpdatesObserver::Uptr RegisterObserver(UpdatesObserver::Uptr observer = nullptr) noexcept = 0;

    /// @brief Rollback all changes executed during the transaction in Key Storage.
    ///        The rollback command permanently cancels all changes made during the transaction in Key Storage.
    ///        A rolled back transaction is completely invisible for all applications.
    /// @param id an ID of a transaction that should be rolled back
    /// @return --
    /// @uptrace{SWS_CRYPT_30125}
    virtual ara::core::Result<void> RollbackTransaction(TransactionId id) noexcept = 0;

    /// @brief Unsubscribe the Update Observer from changes monitoring of the specified slot.
    /// @param slot number of a slot that should be unsubscribed from the updates observing
    /// @return --
    /// @uptrace{SWS_CRYPT_30126}
    virtual ara::core::Result<void> UnsubscribeObserver(KeySlot& slot) noexcept = 0;

    /// @brief Copy-assign another KeyStorageProvider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30222}
    KeyStorageProvider& operator=(const KeyStorageProvider& other) = default;

    /// @brief Move-assign another KeyStorageProvider to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30223}
    KeyStorageProvider& operator=(KeyStorageProvider&& other) = default;
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_KEY_STORAGE_PROVIDER_H