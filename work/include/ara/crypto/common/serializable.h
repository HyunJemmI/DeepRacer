// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_SERIALIZABLE_H
#define ARA_CRYPTO_COMMON_SERIALIZABLE_H

#include <cstdint>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"

namespace ara
{
namespace crypto
{

/// @brief Serializable object interface.
/// @uptrace{SWS_CRYPT_10700}
class Serializable
{
  public:
    /// @brief A container type for the encoding format identifiers.
    /// @uptrace{SWS_CRYPT_10701}
    using FormatId = std::uint32_t;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_10710}
    ~Serializable();

    /// @brief Serialize itself publicly.
    /// @param formatId the Crypto Provider specific identifier of the output format
    /// @return a buffer with the serialized object
    /// @uptrace{SWS_CRYPT_10711}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ExportPublicly(
        FormatId formatId = kFormatDefault) const noexcept = 0;

    /// @brief Serialize itself publicly.
    ///        This method sets the size of the output container according to actually saved value!
    /// @uptrace{SWS_CRYPT_10712}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> ExportPublicly(FormatId formatId = kFormatDefault) const noexcept;

    /// @brief Copy-assign another Serializable to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30204}
    Serializable& operator=(const Serializable& other) = default;

    /// @brief Move-assign another Serializable to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30205}
    Serializable& operator=(Serializable&& other) = default;

    /// @brief Default serialization format.
    /// @uptrace{SWS_CRYPT_10750}
    const FormatId kFormatDefault = 0;

    /// @brief Export DER-encoded value of an object.
    /// @uptrace{SWS_CRYPT_10752}
    const FormatId kFormatDerEncoded = 2;

    /// @brief Export PEM-encoded value of an object.
    /// @uptrace{SWS_CRYPT_10753}
    const FormatId kFormatPemEncoded = 3;

    /// @brief Export only raw value of an object.
    /// @uptrace{SWS_CRYPT_10751}
    const FormatId kFormatRawValueOnly = 1;
};

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_SERIALIZABLE_H