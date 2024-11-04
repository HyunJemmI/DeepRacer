// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_X509_EXTENSIONS_H
#define ARA_CRYPTO_X509_X509_EXTENSIONS_H

#include <memory>

#include "ara/crypto/x509/x509_object.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief Interface of X.509 Extensions.
/// @uptrace{SWS_CRYPT_40500}
class X509Extensions : public X509Object
{
  public:
    /// @brief Shared smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40501}
    using Uptr = std::unique_ptr<X509Extensions>;

    /// @brief Count number of elements in the sequence.
    /// @return number of elements in the sequence
    /// @uptrace{SWS_CRYPT_40511}
    virtual std::size_t Count() const noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_EXTENSIONS_H