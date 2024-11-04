// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H
#define ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/hash_function_ctx.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief General Asymmetric Public Key interface.
/// @uptrace{SWS_CRYPT_22700}
class PublicKey : public RestrictedUseObject
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_22701}
    using Uptrc = std::unique_ptr<const PublicKey>;

    /// @brief Check the key for its correctness.
    /// @param strongCheck the severeness flag that indicates type of the required check: strong (if true) or fast (if
    /// false)
    /// @return true if the key is correct
    /// @uptrace{SWS_CRYPT_22711}
    virtual bool CheckKey(bool strongCheck = true) const noexcept = 0;

    /// @brief Calculate hash of the Public Key value. The original public key value BLOB is available via the
    /// Serializable interface.
    /// @param hashFunc a hash-function instance that should be used the hashing
    /// @return a buffer preallocated for the resulting hash value
    /// @uptrace{SWS_CRYPT_22712}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> HashPublicKey(
        HashFunctionCtx& hashFunc) const noexcept = 0;

    /// @brief Calculate hash of the Public Key value.
    ///        This method sets the size of the output container according to actually saved value!
    ///        The original public key value BLOB is available via the Serializable interface.
    /// @param hashFunc a hash-function instance that should be used the hashing
    /// @return pre-reserved managed container for the resulting hash value
    /// @uptrace{SWS_CRYPT_22713}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> HashPublicKey(HashFunctionCtx& hashFunc) const noexcept;

    /// @brief const object type
    /// @uptrace{SWS_CRYPT_22702}
    const CryptoObjectType kObjectType = CryptoObjectType::kPublicKey;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_PUBLIC_KEY_H