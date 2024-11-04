// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_X509_OBJECT_H
#define ARA_CRYPTO_X509_X509_OBJECT_H

#include "ara/crypto/common/serializable.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief Common interface of all objects created by X.509 Provider.
/// @uptrace{SWS_CRYPT_40900}
class X509Object : public Serializable
{
  public:
    /// @brief Get a reference to X.509 Provider of this object.
    /// @return a reference to X.509 Provider instance that provides this object
    /// @uptrace{SWS_CRYPT_40911}
    virtual X509Provider& MyProvider() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_OBJECT_H