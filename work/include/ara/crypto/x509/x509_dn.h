// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_X509_X509_DN_H
#define ARA_CRYPTO_X509_X509_DN_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/x509_object.h"

namespace ara
{
namespace crypto
{
namespace x509
{

/// @brief Interface of X.509 Distinguished Name (DN).
/// @uptrace{SWS_CRYPT_40400}
class X509DN : public X509Object
{
  public:
    /// @brief Enumeration of DN attributes’ identifiers.
    /// @uptrace{SWS_CRYPT_40403}
    enum class AttributeId : std::uint32_t
    {
        kCommonName = 0,            // Common Name.
        kCountry = 1,               // Country.
        kState = 2,                 // State.
        kLocality = 3,              // Locality.
        kOrganization = 4,          // Organization.
        kOrgUnit = 5,               // Organization Unit.
        kStreet = 6,                // Street.
        kPostalCode = 7,            // Postal Code.
        kTitle = 8,                 // Title.
        kSurname = 9,               // Surname.
        kGivenName = 10,            // Given Name.
        kInitials = 11,             // Initials.
        kPseudonym = 12,            // Pseudonym.
        kGenerationQualifier = 13,  // Generation Qualifiers
        kDomainComponent = 14,      // Domain Component.
        kDnQualifier = 15,          // Distinguished Name Qualifier.
        kEmail = 16,                // E-mail.
        kUri = 17,                  // URI.
        kDns = 18,                  // DNS.
        kHostName = 19,             // Host Name (UNSTRUCTUREDNAME)
        kIpAddress = 20,            // IP Address (UNSTRUCTUREDADDRESS)
        kSerialNumbers = 21,        // Serial Numbers.
        kUserId = 22                // User ID.
    };

    /// @brief Unique smart pointer of the constant interface.
    /// @uptrace{SWS_CRYPT_40402}
    using Uptrc = std::unique_ptr<const X509DN>;

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_40401}
    using Uptr = std::unique_ptr<X509DN>;

    /// @brief Get DN attribute by its ID (this method is applicale to all attributes except kOrgUnit and
    /// kDomainComponent).
    ///        Capacity of the output string must be enough for storing the output value!
    ///        If (attribute == nullptr) then method only returns required buffer capacity.
    /// @param id the identifier of required attribute
    /// @return StringView of the attribute
    /// @uptrace{SWS_CRYPT_40413}
    virtual ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id) const noexcept = 0;

    /// @brief Return DN attribute by its ID and sequential index (this method is applicale to attributes kOrg Unit and
    /// kDomainComponent).
    ///        Capacity of the output string must be enough for storing the output value!
    ///        If (attribute == nullptr) then method only returns required buffer capacity.
    /// @param id the identifier of required attribute
    /// @param index the zero-based index of required component of the attribute
    /// @return StringView of the attribute
    /// @uptrace{SWS_CRYPT_40415}
    virtual ara::core::Result<ara::core::StringView> GetAttribute(AttributeId id, unsigned index) const noexcept = 0;

    /// @brief Get the whole Distinguished Name (DN) as a single string.
    ///        Capacity of the output string must be enough for storing the output value!
    ///        If (dn == nullptr) then method only returns required buffer capacity.
    /// @return StringView of the whole DN string
    /// @uptrace{SWS_CRYPT_40411}
    virtual ara::core::Result<ara::core::StringView> GetDnString() const noexcept = 0;

    /// @brief Check for equality of this and another Distinguished Name (DN) objects.
    /// @param other another instance of DN for comparison
    /// @return true if the provided DN is identical to this one and false otherwise
    /// @uptrace{SWS_CRYPT_40417}
    virtual bool operator==(const X509DN& other) const noexcept = 0;

    /// @brief Check for inequality of this and another Distinguished Name (DN) objects.
    /// @param other another instance of DN for comparison
    /// @return true if the provided DN is not identical to this one and false otherwise
    /// @uptrace{SWS_CRYPT_40418}
    bool operator!=(const X509DN& other) const noexcept;

    /// @brief Set DN attribute by its ID (this method is applicale to all attributes except kOrgUnit and
    /// kDomainComponent).
    /// @param id the identifier of required attribute
    /// @param attribute the attribute value
    /// @return --
    /// @uptrace{SWS_CRYPT_40414}
    virtual ara::core::Result<void> SetAttribute(AttributeId id, ara::core::StringView attribute) const noexcept = 0;

    /// @brief Set DN attribute by its ID and sequential index (this method is applicale to attributes kOrgUnit and
    /// kDomainComponent).
    /// @param id the identifier of required attribute
    /// @param index the zero-based index of required component of the attribute
    /// @param attribute the attribute value
    /// @return --
    /// @uptrace{SWS_CRYPT_40416}
    virtual ara::core::Result<void> SetAttribute(AttributeId id, unsigned index,
                                                 ara::core::StringView attribute) const noexcept = 0;

    /// @brief Set whole Distinguished Name (DN) from a single string. [Error]: SecurityErrorDomain::kUnexpectedValue if
    /// the dn string has incorrect syntax.
    /// @param dn the single string containing the whole DN value in text format
    /// @return --
    /// @uptrace{SWS_CRYPT_40412}
    virtual ara::core::Result<void> SetDn(ara::core::StringView dn) noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_DN_H