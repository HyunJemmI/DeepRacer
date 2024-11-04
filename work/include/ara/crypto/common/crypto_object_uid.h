// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H
#define ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H

#include <cstdint>

#include "ara/crypto/common/uuid.h"

namespace ara
{
namespace crypto
{

/// @brief Definition of Crypto Object Unique Identifier (COUID) type.
/// @uptrace{SWS_CRYPT_10100}
struct CryptoObjectUid
{
    /// @brief Check whether this identifier was generated earlier than the one provided by the argument.
    /// @param anotherId another identifier for the comparison
    /// @return true if this identifier was generated earlier than the anotherId
    /// @uptrace{SWS_CRYPT_10112}
    constexpr bool HasEarlierVersionThan(const CryptoObjectUid& anotherId) const noexcept;

    /// @brief Check whether this identifier was generated later than the one provided by the argument.
    /// @param anotherId another identifier for the comparison
    /// @return true if this identifier was generated later than the anotherId
    /// @uptrace{SWS_CRYPT_10113}
    constexpr bool HasLaterVersionThan(const CryptoObjectUid& anotherId) const noexcept;

    /// @brief Check whether this identifier has a common source with the one provided by the argument.
    /// @param anotherId another identifier for the comparison
    /// @return true if both identifiers has common source (identical value of the mGeneratorUid field)
    /// @uptrace{SWS_CRYPT_10111}
    constexpr bool HasSameSourceAs(const CryptoObjectUid& anotherId) const noexcept;

    /// @brief Check whether this identifier is "Nil".
    /// @return true if this identifier is "Nil" and false otherwise
    /// @uptrace{SWS_CRYPT_10114}
    bool IsNil() const noexcept;

    /// @brief Check whether this object’s generator identifier is "Nil".
    /// @return true if this identifier is "Nil" and false otherwise
    bool SourceIsNil() const noexcept;

    /// @brief UUID of a generator that has produced this COUID. This UUID can be associated with HSM, physical host/ECU
    /// or VM.
    /// @uptrace{SWS_CRYPT_10101}
    Uuid mGeneratorUid;

    /// @brief Sequential value of a steady timer or simple counter, representing version of correspondent Crypto
    /// Object.
    /// @uptrace{SWS_CRYPT_10102}
    std::uint64_t mVersionStamp = 0U;
};

/// @brief Comparison operator "equal" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if all members’ values of lhs is equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10150}
constexpr bool operator==(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

/// @brief Comparison operator "less than" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is less than rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10151}
constexpr bool operator<(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

/// @brief Comparison operator "greater than" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is greater than rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10152}
constexpr bool operator>(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

/// @brief Comparison operator "not equal" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if at least one member of lhs has a value not equal to correspondent member of rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10153}
constexpr bool operator!=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

/// @brief Comparison operator "less than or equal" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is less than or equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10154}
constexpr bool operator<=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

/// @brief Comparison operator "greater than or equal" for CryptoObjectUid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is greater than or equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10155}
constexpr bool operator>=(const CryptoObjectUid& lhs, const CryptoObjectUid& rhs) noexcept;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_CRYPTO_OBJECT_UID_H