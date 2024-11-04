// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_OCSP_RESPONSE_H
#define ARA_CRYPTO_X509_OCSP_RESPONSE_H

#include <cstdint>

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief On-line Certificate Status Protocol Response.
/// @uptrace{SWS_CRYPT_40800}
class OcspResponse : public X509Object
{
  public:
    /// @brief On-line Certificate Status Protocol (OCSP) Certificate Status.
    /// @uptrace{SWS_CRYPT_40002}
    enum class OcspCertStatus : std::uint32_t
    {
        kGood = 0,     // The certificate is not revoked.
        kRevoked = 1,  // The certificate has been revoked (either permanantly or temporarily (on hold))
        kUnknown = 2   // The responder doesn’t know about the certificate being requested.
    };

    /// @brief On-line Certificate Status Protocol (OCSP) Response Status.
    /// @uptrace{SWS_CRYPT_40001}
    enum class OcspResponseStatus : std::uint32_t
    {
        kSuccessful = 0,        // Response has valid confirmations.
        kMalformedRequest = 1,  // Illegal confirmation request.
        kInternalError = 2,     // Internal error in issuer.
        kTryLater = 3,          // Try again later.
        kSigRequired = 5,       // Must sign the request.
        kUnauthorized = 6       // Request unauthorized.
    };

    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40802}
    using Uptrc = std::unique_ptr<const OcspResponse>;

    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40801}
    using Uptr = std::unique_ptr<OcspResponse>;

    /// @brief Get version of the OCSP response format.
    /// @return OCSP response format version
    /// @uptrace{SWS_CRYPT_40811}
    virtual std::uint32_t Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_OCSP_RESPONSE_H