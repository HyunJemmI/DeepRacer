// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H
#define ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H

#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{
namespace keys
{

/// @brief Properties of current Key Slot Content, i.e. of a current instance stored to the Key Slot.
///        A value of the mAllowedUsage field is bitwise AND of the common usage flags defined at run-time and
///        the usage flags defined by the UserPermissions prototype for current "Actor".
/// @uptrace{SWS_CRYPT_30500}
struct KeySlotContentProps
{
    /// @brief shared pointer of interface
    /// @uptrace{SWS_CRYPT_30511}
    using Uptr = std::unique_ptr<KeySlotContentProps>;
    /// @brief set content properties
    /// @uptrace{SWS_CRYPT_30510}
    KeySlotContentProps() = default;
    /// @brief Cryptoalgorithm of actual object stored to the slot.
    /// @uptrace{SWS_CRYPT_30503}
    CryptoAlgId mAlgId;
    /// @brief Actual size of an object currently stored to the slot.
    /// @uptrace{SWS_CRYPT_30505}
    std::size_t mObjectSize;
    /// @brief Actual type of an object stored to the slot.
    /// @uptrace{SWS_CRYPT_30508}
    CryptoObjectType mObjectType;
    /// @brief UID of a Crypto Object stored to the slot.
    /// @uptrace{SWS_CRYPT_30501}
    CryptoObjectUid mObjectUid;
    /// @brief Actual usage restriction flags of an object stored to the slot for the current "Actor".
    /// @uptrace{SWS_CRYPT_30506}
    AllowedUsageFlags mContentAllowedUsage;
};

/// @brief Comparison operator "equal" for KeySlotContentProps operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if all members’ values of lhs is equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_30550}
constexpr bool operator==(const KeySlotContentProps& lhs, const KeySlotContentProps& rhs) noexcept;

/// @brief Comparison operator "not equal" for KeySlotContentProps operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if at least one member of lhs has a value not equal to correspondent member of rhs, and false otherwise
/// @uptrace{SWS_CRYPT_303551}
constexpr bool operator!=(const KeySlotContentProps& lhs, const KeySlotContentProps& rhs) noexcept;

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_KEYS_KEY_SLOT_CONTENT_PROPS_H