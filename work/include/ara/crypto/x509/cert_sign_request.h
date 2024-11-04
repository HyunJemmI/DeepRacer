// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H
#define ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/x509/basic_cert_info.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief Certificate Signing Request (CSR) object interface
///        This interface is dedicated for complete parsing of the request content.
/// @uptrace{SWS_CRYPT_40300}
class CertSignRequest : public BasicCertInfo
{
  public:
    /// @brief Unique smart pointer of the constant interface.
    /// @uptrace{SWS_CRYPT_40301}
    using Uptrc = std::unique_ptr<const CertSignRequest>;

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40302}
    using Uptr = std::unique_ptr<CertSignRequest>;

    /// @brief Verifies self-signed signature of the certificate request.
    /// @return true if the signature is correct
    /// @uptrace{SWS_CRYPT_40311}
    virtual bool Verify() const noexcept = 0;

    /// @brief Export this certificate signing request in DER encoded ASN1 format.
    ///        Note: this is the CSR that can be sent to the CA for obtaining the certificate.
    /// @return a buffer with the formatted CSR
    /// @uptrace{SWS_CRYPT_40313}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportASN1CertSignRequest() noexcept = 0;

    /// @brief Return signature object of the request.
    /// @return signature object of the request
    /// @uptrace{SWS_CRYPT_40315}
    virtual const ara::crypto::cryp::Signature& GetSignature() const noexcept = 0;

    /// @brief Return format version of the certificate request.
    /// @return format version of the certificate request
    /// @uptrace{SWS_CRYPT_40314}
    virtual unsigned Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_CERT_SIGN_REQUEST_H
