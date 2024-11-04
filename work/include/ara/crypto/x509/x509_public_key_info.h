// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H
#define ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H

#include "ara/core/result.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/cryp/cryobj/public_key.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief X.509 Public Key Information interface.
/// @uptrace{SWS_CRYPT_24400}
class X509PublicKeyInfo : public Serializable
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_24401}
    using Uptrc = std::unique_ptr<const X509PublicKeyInfo>;

    /// @brief Get public key object of the subject. Created PublicKey object is session and non-exportable,
    ///        because generic X.509 certificate or certificate signing request (CSR) doesn’t have COUID of the public
    ///        key, therefore it should be saved or transmitted only as a part of correspondent certificate or CSR.
    /// @return unique smart pointer to the created public key of the subject
    /// @uptrace{SWS_CRYPT_24414}
    virtual ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc> GetPublicKey() const noexcept = 0;

    /// @brief Get an ID of hash algorithm required by current signature algorithm.
    /// @return required hash algorithm ID or kAlgIdAny if the signature algorithm specification does not include a
    /// concrete hash function
    /// @uptrace{SWS_CRYPT_24412}
    virtual CryptoAlgId GetRequiredHashAlgId() const noexcept = 0;

    /// @brief Get the hash size required by current signature algorithm.
    /// @return required hash size in bytes
    /// @uptrace{SWS_CRYPT_24411}
    virtual std::size_t GetRequiredHashSize() const noexcept = 0;

    /// @brief Get size of the signature value produced and required by the current algorithm.
    /// @return size of the signature value in bytes
    /// @uptrace{SWS_CRYPT_24413}
    virtual std::size_t GetSignatureSize() const noexcept = 0;

    /// @brief Get the CryptoPrimitiveId instance of this class.
    /// @return --
    /// @uptrace{SWS_CRYPT_24410}
    virtual ara::crypto::cryp::CryptoPrimitiveId::Uptrc GetAlgorithmId() = 0;

    /// @brief Verify the sameness of the provided and kept public keys. This method compare the public key values only.
    /// @param publicKey the public key object for comparison
    /// @return true if values of the stored public key and object provided by the argument are identical and false
    /// otherwise
    /// @uptrace{SWS_CRYPT_24415}
    virtual bool IsSameKey(const ara::crypto::cryp::PublicKey& publicKey) const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_PUBLIC_KEY_INFO_H