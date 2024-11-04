// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_UUID_H
#define ARA_CRYPTO_COMMON_UUID_H

#include <cstdint>

namespace ara
{
namespace crypto
{

/// @brief Definition of Universally Unique Identifier (UUID) type.
///        Independently from internal definition details of this structure, it’s size must be 16 bytes and entropy of
///        this ID should be close to 128 bit
/// @uptrace{SWS_CRYPT_10400}
struct Uuid
{
    /// @brief Check whether this identifier is the "Nil UUID" (according to RFC4122).
    /// @return true if this identifier is "Nil" and false otherwise
    /// @uptrace{SWS_CRYPT_10411}
    bool IsNil() const noexcept;

    /// @brief Less significant QWORD.
    /// @uptrace{SWS_CRYPT_10412}
    std::uint64_t mQwordLs = 0U;

    /// @brief Most significant QWORD.
    /// @uptrace{SWS_CRYPT_10413}
    std::uint64_t mQwordMs = 0u;
};

/// @brief Comparison operator "less than" for Uuid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is less than rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10452}
constexpr bool operator<(const Uuid& lhs, const Uuid& rhs) noexcept;

/// @brief Comparison operator "greater than" for Uuid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is greater than rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10453}
constexpr bool operator>(const Uuid& lhs, const Uuid& rhs) noexcept;

/// @brief Comparison operator "not equal" for Uuid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is not equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10454}
constexpr bool operator!=(const Uuid& lhs, const Uuid& rhs) noexcept;

/// @brief Comparison operator "less than or equal" for Uuid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is less than or equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10455}
constexpr bool operator<=(const Uuid& lhs, const Uuid& rhs) noexcept;

/// @brief Comparison operator "greater than or equal" for Uuid operands.
/// @param lhs left-hand side operand
/// @param rhs right-hand side operand
/// @return true if a binary representation of lhs is greater than or equal to rhs, and false otherwise
/// @uptrace{SWS_CRYPT_10456}
constexpr bool operator>=(const Uuid& lhs, const Uuid& rhs) noexcept;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_UUID_H