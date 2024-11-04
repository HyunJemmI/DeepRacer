// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_CERTIFICATE_H
#define ARA_CRYPTO_X509_CERTIFICATE_H

#include <memory>

#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/x509/basic_cert_info.h"
#include "ara/crypto/x509/x509_dn.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief X.509 Certificate interface.
/// @uptrace{SWS_CRYPT_40200}
class Certificate : public BasicCertInfo
{
  public:
    /// @brief Certificate verification status.
    /// @uptrace{SWS_CRYPT_40203}
    enum class Status : std::uint32_t
    {
        kValid = 0,    // The certificate is valid.
        kInvalid = 1,  // The certificate is invalid.
        kUnknown = 2,  // Status of the certificate is unknown yet.
        kNoTrust =
            3,  // The certificate has correct signature, but the ECU has no a root of trust for this certificate.
        kExpired =
            4,       // The certificate has correct signature, but it is already expired (its validity period has ended)
        kFuture = 5  // The certificate has correct signature, but its validity period is not started yet.
    };

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40202}
    using Uptrc = std::unique_ptr<const Certificate>;

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40201}
    using Uptr = std::unique_ptr<Certificate>;

    /// @brief Get the DER encoded AuthorityKeyIdentifier of this certificate.
    ///        If (id.empty() == true) then this method only returns required size of the output buffer.
    /// @return size of the DER encoded AuthorityKeyIdentifier in bytes
    /// @uptrace{SWS_CRYPT_40217}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> AuthorityKeyId() const noexcept = 0;

    /// @brief Get the "Not After" of the certificate.
    /// @return "Not After" of the certificate
    /// @uptrace{SWS_CRYPT_40215}
    virtual time_t EndTime() const noexcept = 0;

    /// @brief Calculate a fingerprint from the whole certificate.
    ///        The produced fingerprint value saved to the output buffer starting from leading bytes of the hash value.
    ///        If the capacity of the output buffer is less than the digest size then the digest will be truncated and
    ///        only leading bytes will be saved. If the capacity of the output buffer is higher than the digest size
    ///        then only leading bytes of the buffer will be updated.
    /// @param fingerprint output buffer for the fingerprint storage
    /// @param hashCtx an initialized hash function context
    /// @return number of bytes actually saved to the output buffer
    /// @uptrace{SWS_CRYPT_40220}
    virtual ara::core::Result<std::size_t> GetFingerprint(ReadWriteMemRegion fingerprint,
                                                          cryp::HashFunctionCtx& hashCtx) const noexcept = 0;

    /// @brief Return last verification status of the certificate.
    /// @return the certificate verification status
    /// @uptrace{SWS_CRYPT_40221}
    virtual Status GetStatus() const noexcept = 0;

    /// @brief Check whether this certificate belongs to a root CA.
    /// @return true if the TrustMaster has set this certificate as root
    /// @uptrace{SWS_CRYPT_40212}
    virtual bool IsRoot() const noexcept = 0;

    /// @brief Get the issuer certificate DN.
    /// @return Issuer DN of this certificate
    /// @uptrace{SWS_CRYPT_40213}
    virtual const X509DN& IssuerDn() const = 0;

    /// @brief Get the serial number of this certificate. If (sn.empty() == true) then this method only returns required
    /// size of the output buffer.
    /// @return size of the certificate serial number in bytes
    /// @uptrace{SWS_CRYPT_40216}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SerialNumber() const noexcept = 0;

    /// @brief Get the "Not Before" of the certificate.
    /// @return "Not Before" of the certificate
    /// @uptrace{SWS_CRYPT_40214}
    virtual time_t StartTime() const noexcept = 0;

    /// @brief Get the DER encoded SubjectKeyIdentifier of this certificate.
    ///        If (id.empty() == true) then this method only returns required size of the output buffer.
    /// @return size of the DER encoded SubjectKeyIdentifier in bytes
    /// @uptrace{SWS_CRYPT_40218}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> SubjectKeyId() const noexcept = 0;

    /// @brief Verify signature of the certificate. Call with (caCert == nullptr) is applicable only if this is a
    /// certificate of a root CA.
    /// @param caCert the optional pointer to a Certification Authority certificate used for signature of the current
    /// one
    /// @return true if this certificate was verified successfully and false otherwise
    /// @uptrace{SWS_CRYPT_40219}
    virtual bool VerifyMe(ara::core::Optional<const Certificate> caCert) const noexcept = 0;

    /// @brief Get the X.509 version of this certificate object.
    /// @return X.509 version
    /// @uptrace{SWS_CRYPT_40211}
    virtual std::uint32_t X509Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_CERTIFICATE_H