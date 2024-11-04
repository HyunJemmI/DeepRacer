// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_BASIC_CERT_INFO_H
#define ARA_CRYPTO_X509_BASIC_CERT_INFO_H

#include <cstdint>

#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/x509_object.h"
#include "ara/crypto/x509/x509_public_key_info.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief Basic Certificate Information interface.
/// @uptrace{SWS_CRYPT_40100}
class BasicCertInfo : public X509Object
{
  public:
    /// @brief X.509 v3 Key Constraints type definition.
    /// @uptrace{SWS_CRYPT_40101}
    using KeyConstraints = std::uint32_t;

    /// @brief Get the key constraints for the key associated with this PKCS#10 object.
    /// @return key constraints
    /// @uptrace{SWS_CRYPT_40115}
    virtual KeyConstraints GetConstraints() const noexcept = 0;

    /// @brief Get the constraint on the path length defined in the Basic Constraints extension.
    /// @return certification path length limit
    /// @uptrace{SWS_CRYPT_40114}
    virtual std::uint32_t GetPathLimit() const noexcept = 0;

    /// @brief Check whether the CA attribute of X509v3 Basic Constraints is true (i.e. pathlen=0).
    /// @return true if it is a CA request and false otherwise
    /// @uptrace{SWS_CRYPT_40113}
    virtual bool IsCa() const noexcept = 0;

    /// @brief Get the subject DN.
    /// @return subject DN
    /// @uptrace{SWS_CRYPT_40112}
    virtual const X509DN& SubjectDn() const noexcept = 0;

    /// @brief Load the subject public key information object to realm of specified crypto provider.
    ///        If (cryptoProvider == nullptr) then X509PublicKeyInfo object will be loaded in realm of the Stack-default
    ///        Crypto Provider.
    /// @param cryptoProvider unique pointer of a target Crypto Provider, where the public key will be used
    /// @return constant reference of the subject public key interface
    /// @uptrace{SWS_CRYPT_40111}
    virtual const X509PublicKeyInfo& SubjectPubKey(
        cryp::CryptoProvider::Uptr cryptoProvider = nullptr) const noexcept = 0;

    /// @brief The key can be used for Certificates Revokation Lists (CRL) signing.
    /// @uptrace{SWS_CRYPT_40157}
    const KeyConstraints kConstrCrlSign = 0x0200;

    /// @brief The key can be used for data encipherment.
    /// @uptrace{SWS_CRYPT_40154}
    const KeyConstraints kConstrDataEncipherment = 0x1000;

    /// @brief The enciphermet key can be used for deciphering only.
    /// @uptrace{SWS_CRYPT_40159}
    const KeyConstraints kConstrDecipherOnly = 0x0080;

    /// @brief The key can be used for digital signature production.
    /// @uptrace{SWS_CRYPT_40151}
    const KeyConstraints kConstrDigitalSignature = 0x8000;

    /// @brief The enciphermet key can be used for enciphering only.
    /// @uptrace{SWS_CRYPT_40158}
    const KeyConstraints kConstrEncipherOnly = 0x0100;

    /// @brief The key can be used for a key agreement protocol execution.
    /// @uptrace{SWS_CRYPT_40155}
    const KeyConstraints kConstrKeyAgreement = 0x0800;

    /// @brief The key can be used for certificates signing.
    /// @uptrace{SWS_CRYPT_40156}
    const KeyConstraints kConstrKeyCertSign = 0x0400;

    /// @brief The key can be used for key encipherment.
    /// @uptrace{SWS_CRYPT_40153}
    const KeyConstraints kConstrKeyEncipherment = 0x2000;

    /// @brief The key can be used in cases requiring the "non-repudiation" guarantee.
    /// @uptrace{SWS_CRYPT_40152}
    const KeyConstraints kConstrNonRepudiation = 0x4000;

    /// @brief No key constraints.
    /// @uptrace{SWS_CRYPT_40150}
    const KeyConstraints kConstrNone = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_BASIC_CERT_INFO_H