// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_KEYSLOT_H
#define ARA_CRYPTO_KEYS_KEYSLOT_H

#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/io_interface.h"
#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Key slot port-prototype interface. This class enables access to a physicl key-slot.
/// @uptrace{SWS_CRYPT_30400}
class KeySlot
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_30402}
    using Uptr = std::unique_ptr<KeySlot>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_30401}
    virtual ~KeySlot() noexcept = default;

    /// @brief Clear the content of this key-slot. This method must perform a secure cleanup without the ability to
    /// restore the object data!
    ///        This method may be used for atomic update of a key slot scoped to some transaction.
    ///        In such case the the slot will be updated only after correspondent call of CommitTransaction().
    /// @return --
    /// @uptrace{SWS_CRYPT_30405}
    virtual ara::core::Result<void> Clear() noexcept = 0;

    /// @brief Get an actual properties of a content in the key slot.
    ///        If this method called by a "User" Actor then always: props.exportability == false.
    /// @return --
    /// @uptrace{SWS_CRYPT_30408}
    virtual ara::core::Result<KeySlotContentProps> GetContentProps() const noexcept = 0;

    /// @brief Retrieve an instance of the CryptoProvider that owns this KeySlot.
    ///        Any key slot always has an associated default Crypto Provider that can serve this key slot.
    ///        In the simplest case all key slots can be served by a single Crypto Provider installed on the Adaptive
    ///        Platform. But in a more complicated case a few different Crypto Providers may coexist in the system, for
    ///        example if ECU has one or a few HSMs and software cryptography implementation too, and each of them has
    ///        own physical key storage. In such case different dedicated Crypto Providers may serve mentioned HSMs and
    ///        the software implementation.
    /// @return a unique_pointer to the CryptoProvider to be used with this KeySlot
    /// @uptrace{SWS_CRYPT_30403}
    virtual ara::core::Result<cryp::CryptoProvider::Uptr> MyProvider() const noexcept = 0;

    /// @brief Get the prototyped properties of the key slot.
    /// @return --
    /// @uptrace{SWS_CRYPT_30407}
    virtual ara::core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept = 0;

    /// @brief Check the slot for emptiness.
    /// @return true if the slot is empty or false otherwise
    /// @uptrace{SWS_CRYPT_30404}
    virtual bool IsEmpty() const noexcept = 0;

    /// @brief Open this key slot and return an IOInterface to its content.
    ///        If the UpdatesObserver interface was provided to the call of RegisterObserver() then the
    ///        UpdatesObserver::OnUpdate() method should be called by Key Storage engine (in a dedicated thread) every
    ///        time when this slot is updated (and become visible for "Users"). Monitoring of the opened key slot will
    ///        be continued even after destruction of the returned TrustedContainer, because content of the slot may be
    ///        loaded to volatile memory (as a CryptoObject or to a CryptoContext of a crypto primitive), but the
    ///        TrustedContainer may be destroyed after this. Therefore if you need to terminate monitoring of the key
    ///        slot then you should directly call method UnsubscribeObserver(SlotNumber).
    /// @param subscribeForUpdates if this flag is true then the UpdatesObserver instance
    ///        (previously registered by a call of the method RegisterObserver()) will be subscribed for updates of the
    ///        opened key slot
    /// @param writeable indicates whether the key-slot shall be opened read-only (default) or with write access
    /// @return an unique smart pointer to the IOInterface associated with the slot content
    /// @uptrace{SWS_CRYPT_30409}
    virtual ara::core::Result<IOInterface::Uptr> Open(bool subscribeForUpdates = false,
                                                      bool writeable = false) const noexcept = 0;

    /// @brief Save the content of a provided source IOInterface to this key-slot.
    ///        The source container may represent a volatile trusted container or another KeySlot
    ///        This method may be used for atomic update of a key slot scoped to some transaction.
    ///        In such case the the slot will be updated only after correspondent call of CommitTransaction().
    /// @param container the source IOInterface
    /// @return true if successfully saved
    /// @uptrace{SWS_CRYPT_30406}
    virtual ara::core::Result<void> SaveCopy(const IOInterface& container) noexcept = 0;

    /// @brief Copy-assign another KeySlot to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30220}
    KeySlot& operator=(const KeySlot& other) = default;

    /// @brief Move-assign another KeySlot to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30221}
    KeySlot& operator=(KeySlot&& other) = default;
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_KEYSLOT_H
