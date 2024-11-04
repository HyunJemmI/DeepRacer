// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H
#define ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H

#include <cstdint>
#include <memory>

#include "ara/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Secret Seed object interface. This object contains a raw bit sequence of specific length (without any
/// filtering of allowed/disallowed values)!
///        The secret seed value can be loaded only to a non-key input of a cryptographic transformation context (like
///        IV/salt/nonce)! Bit length of the secret seed is specific to concret crypto algorithm and corresponds to
///        maximum of its input/output/salt block-length.
/// @uptrace{SWS_CRYPT_23000}
class SecretSeed : public RestrictedUseObject
{
  public:
    /// @brief Unique smart pointer of a constant interface instance.
    /// @uptrace{SWS_CRYPT_23001}
    using Uptrc = std::unique_ptr<const SecretSeed>;

    /// @brief Unique smart pointer of a volatile interface instance.
    /// @uptrace{SWS_CRYPT_23002}
    using Uptr = std::unique_ptr<SecretSeed>;

    /// @brief Clone this Secret Seed object to new session object.
    ///        Created object instance is session and non-exportable, AllowedUsageFlags attribute of the "cloned" object
    ///        is identical to this attribute of the source object! If size of the xorDelta argument is less than the
    ///        value size of this seed then only correspondent number of leading bytes of the original seed should be
    ///        XOR-ed, but the rest should be copied without change. If size of the xorDelta argument is larger than the
    ///        value size of this seed then extra bytes of the xorDelta should be ignored.
    /// @param xorDelta optional "delta" value that must be XOR-ed with the "cloned" copy of the original seed
    /// @return unique smart pointer to "cloned" session SecretSeed object
    /// @uptrace{SWS_CRYPT_23011}
    virtual ara::core::Result<SecretSeed::Uptr> Clone(
        ReadOnlyMemRegion xorDelta = ReadOnlyMemRegion()) const noexcept = 0;

    /// @brief Set value of this seed object as a "jump" from an initial state to specified number of steps,
    ///        according to "counting" expression defined by a cryptographic algorithm associated with this object.
    ///        steps may have positive and negative values that correspond to forward and backward direction of the
    ///        "jump" respectively, but 0 value means only copy from value to this seed object. Seed size of the from
    ///        argument always must be greater or equal of this seed size.
    /// @param from source object that keeps the initial value for jumping from
    /// @param steps number of steps for the "jump"
    /// @return reference to this updated object
    /// @uptrace{SWS_CRYPT_23012}
    virtual ara::core::Result<void> JumpFrom(const SecretSeed& from, std::int64_t steps) noexcept = 0;

    /// @brief Set value of this seed object as a "jump" from it’s current state to specified number of steps,
    ///        according to "counting" expression defined by a cryptographic algorithm associated with this object.
    ///        steps may have positive and negative values that correspond to forward and backward direction of the
    ///        "jump" respectively, but 0 value means no changes of the current seed value.
    /// @param steps number of "steps" for jumping (forward or backward) from the current state
    /// @return reference to this updated object
    /// @uptrace{SWS_CRYPT_23014}
    virtual SecretSeed& Jump(std::int64_t steps) noexcept = 0;

    /// @brief Set next value of the secret seed according to "counting" expression defined by a cryptographic algorithm
    /// associated with this object.
    ///        If the associated cryptographic algorithm doesn’t specify a "counting" expression
    ///        then generic increment operation must be implemented as default (little-endian notation, i.e. first byte
    ///        is least significant).
    /// @return --
    /// @uptrace{SWS_CRYPT_23013}
    virtual SecretSeed& Next() noexcept = 0;

    /// @brief XOR value of this seed object with another one and save result to this object.
    ///        If seed sizes in this object and in the source argument are different then only correspondent number of
    ///        leading bytes in this seed object should be updated.
    /// @param source right argument for the XOR operation
    /// @return reference to this updated object
    /// @uptrace{SWS_CRYPT_23015}
    virtual SecretSeed& operator^=(const SecretSeed& source) noexcept = 0;

    /// @brief XOR value of this seed object with provided memory region and save result to this object.
    ///        If seed sizes in this object and in the source argument are different then only correspondent number of
    ///        leading bytes of this seed object should be updated.
    /// @param source right argument for the XOR operation
    /// @return reference to this updated object
    /// @uptrace{SWS_CRYPT_23016}
    virtual SecretSeed& operator^=(ReadOnlyMemRegion source) noexcept = 0;

    /// @brief Static mapping of this interface to specific value of CryptoObjectType enumeration.
    /// @uptrace{SWS_CRYPT_23003}
    const CryptoObjectType kObjectType = CryptoObjectType::kSecretSeed;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_SECRET_SEED_H