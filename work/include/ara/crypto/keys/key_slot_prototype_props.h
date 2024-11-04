// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H
#define ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H

#include <cstdint>
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Prototyped Properties of a Key Slot.
/// @uptrace{SWS_CRYPT_30300}
struct KeySlotPrototypeProps
{
    /// @brief --
    /// @uptrace{SWS_CRYPT_30302}
    using Uptr = std::unique_ptr<KeySlotPrototypeProps>;
    /// @brief --
    /// @uptrace{SWS_CRYPT_30301}
    KeySlotPrototypeProps() = default;
    /// @brief Cryptoalgorithm restriction (kAlgIdAny means without restriction). The algorithm can be specified
    /// partially: family & length, mode, padding.
    /// @uptrace{SWS_CRYPT_30306}
    CryptoAlgId mAlgId;
    /// @brief Indicates whether FC Crypto shall allocate sufficient storage space for a shadow copy of this KeySlot.
    /// @uptrace{SWS_CRYPT_30309}
    bool mAllocateSpareSlot;
    /// @brief Indicates whether the content of this key-slot may be changed, e.g. from storing a symmetric key to
    /// storing an RSA key
    ///        If this is set to false, then the mObjectType of this KeySlotPrototypeProps must be a) valid and b)
    ///        cannot be changed (i.e. only objects of mObjectType may be stored in this key-slot).
    /// @uptrace{SWS_CRYPT_30310}
    bool mAllowContentTypeChange;
    /// @brief Indicates how the content may be used. The following use cases of this attribute are considered:
    ///        the object to be stored in this key-slot has it’s AllowedUsageFlags set to kAllowPrototypedOnly.
    ///        In this case this attribute must be observed when loading the content into a runtime instance
    ///        (e.g. the AllowedUsageFlags of a SymmetricKey object should be set according to this attribute)
    ///        mMaxUpdatesAllowed==0, in this case the content is provided during production while the AllowedUsageFlags
    ///        is modeled using this attribute when this key-slot is flexibly updated the runtime object’s
    ///        AllowedUsageFlags override this attribute upon a later loading from this key-slot
    /// @uptrace{SWS_CRYPT_30313}
    AllowedUsageFlags mContentAllowedUsage;
    /// @brief Indicates whether the key-slot content may be exported.
    /// @uptrace{SWS_CRYPT_30312}
    bool mExportAllowed;
    /// @brief Specifies how many times this key-slot may be updated, e.g.:
    ///        a value of 0 means the key-slot content will be pre-set during production a value of 1 means
    ///        the key-slot content can be updated only once ("OTP") a negative value means the key-slot content can be
    ///        updated inifinitely
    /// @uptrace{SWS_CRYPT_30311}
    std::int32_t mMaxUpdateAllowed;
    /// @brief Key-slot type configuration: all key-slots used by the adaptive machine to provide serives such
    ///        as secure communication, diagnostics, updates, secure storage etc. shall use the type kMachine.
    ///        All key-slots that will be used by the adaptive user application must use kApplication.
    ///        A key-manager user application may define kMachine key-slots as well; in this case the integrator must
    ///        match a corresponding machine key-slot to be managed.
    /// @uptrace{SWS_CRYPT_30305}
    KeySlotType mSlotType;
    /// @brief Capacity of the slot in bytes.
    /// @uptrace{SWS_CRYPT_30307}
    std::size_t mSlotCapacity;
    /// @brief Restriction of an object type that can be stored the slot. If this field contains
    /// CryptoObjectType::kUnknown then without restriction of the type.
    /// @uptrace{SWS_CRYPT_30308}
    CryptoObjectType mObjectType;
};

/// @brief Comparison operator "equal" for KeySlotPrototypeProps operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if all members’ values of lhs is equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_30350}
constexpr bool operator==(const KeySlotPrototypeProps& lhs, const KeySlotPrototypeProps& rhs) noexcept;

/// @brief Comparison operator "not equal" for KeySlotPrototypeProps operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if at least one member of lhs has a value not equal to correspondent member of rhs, and false otherwise
/// @uptrace{SWS_CRYPT_30351}
constexpr bool operator!=(const KeySlotPrototypeProps& lhs, const KeySlotPrototypeProps& rhs) noexcept;

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_KEY_SLOT_PROTOTYPE_PROPS_H