// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_OCSP_REQUEST_H
#define ARA_CRYPTO_X509_OCSP_REQUEST_H

#include <memory>

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief On-line Certificate Status Protocol Request.
/// @uptrace{SWS_CRYPT_40700}
class OcspRequest : public X509Object
{
  public:
    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40702}
    using Uptrc = std::unique_ptr<const OcspRequest>;

    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40701}
    using Uptr = std::unique_ptr<OcspRequest>;

    /// @brief Get version of the OCSP request format.
    /// @return OCSP request format version
    /// @uptrace{SWS_CRYPT_40711}
    virtual std::uint32_t Version() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_OCSP_REQUEST_H