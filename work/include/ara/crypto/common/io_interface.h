// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_IO_INTERFACE_H
#define ARA_CRYPTO_COMMON_IO_INTERFACE_H

#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
namespace crypto
{

/// @brief Formal interface of an IOInterface is used for saving and loading of security objects.
///        Actual saving and loading should be implemented by internal methods known to a trusted pair of
///        Crypto Provider and Storage Provider. Each object should be uniquely identified by its type and
///        Crypto Object Unique Identifier (COUID). This interface suppose that objects in the container
///        are compressed i.e. have a minimal size optimized for.
/// @uptrace{SWS_CRYPT_10800}
class IOInterface
{
    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_10810}
    virtual ~IOInterface() noexcept = default;

    /// @brief Return actual allowed key/seed usage flags defined by the key slot prototype for this "Actor"
    ///        and current content of the container. Volatile containers don’t have any prototyped restrictions,
    ///        but can have restrictions defined at run-time for a current instance of object. A value returned
    ///        by this method is bitwise AND of the common usage flags defined at run-time and the usage
    ///        flags defined by the UserPermissions prototype for current "Actor". This method is especially
    ///        useful for empty permanent prototyped containers.
    /// @return allowed key/seed usage flags
    /// @uptrace{SWS_CRYPT_10819}
    virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

    /// @brief Return capacity of the underlying resource.
    /// @return capacity of the underlying buffer of this IOInterface (in bytes)
    /// @uptrace{SWS_CRYPT_10813}
    virtual std::size_t GetCapacity() const noexcept = 0;

    /// @brief Return the CryptoObjectType of the object referenced by this IOInterface.
    /// @return the CryptoObjectType stored inside the referenced resource
    /// @uptrace{SWS_CRYPT_10812}
    virtual CryptoObjectType GetCryptoObjectType() const noexcept = 0;

    /// @brief Return COUID of an object stored to this IOInterface.
    ///        If the container is empty then this method returns CryptoObjectType::KUndefined.
    ///        Unambiguous identification of a crypto object requires both components: CryptoObjectUid and
    ///        CryptoObjectType.
    /// @return type of the content stored in the container
    /// @uptrace{SWS_CRYPT_10811}
    virtual CryptoObjectUid GetObjectId() const noexcept = 0;

    /// @brief Return size of an object payload stored in the underlying buffer of this IOInterface.
    ///        If the container is empty then this method returns 0. Returned value does not take into account the
    ///        object’s meta-information properties, but their size is fixed and common for all crypto objects
    ///        independently from their actual type. space for an object’s meta-information automatically,
    ///        according to their implementation details.
    /// @return size of an object payload stored in the underlying buffer of this IOInterface (in bytes)
    /// @uptrace{SWS_CRYPT_10817}
    virtual std::size_t GetPayloadSize() const noexcept = 0;

    /// @brief Get vendor specific ID of the primitive.
    /// @return the binary Crypto Primitive ID
    /// @uptrace{SWS_CRYPT_10822}
    virtual CryptoAlgId GetPrimitiveId() const noexcept = 0;

    /// @brief Return content type restriction of this IOInterface. If
    /// KeySlotPrototypeProps::mAllowContentTypeChange==TRUE, then kUndefined shall be returned.
    ///        If a container has a type restriction different from CryptoObjectType::kUndefined then only objects of
    ///        the mentioned type can be saved to this container. Volatile containers don’t have any content type
    ///        restrictions.
    /// @return an object type of allowed content (CryptoObjectType::kUndefined means without restriction)
    /// @uptrace{SWS_CRYPT_10818}
    virtual CryptoObjectType GetTypeRestriction() const noexcept = 0;

    /// @brief Return the "exportable" attribute of an object stored to the container.
    ///        The exportability of an object doesn’t depend from the volatility of its container.
    /// @return true if an object stored to the container has set the "exportable" attribute
    /// @uptrace{SWS_CRYPT_10816}
    virtual bool IsObjectExportable() const noexcept = 0;

    /// @brief Return the "session" (or "temporary") attribute of an object as set
    ///        e.g. by KeyDerivationFunctionCtx::DeriveKey().
    ///        A "session" object can be stored to a VolatileTrustedContainer only!
    ///        If this IOInterface is linked to a KeySlot this returns always false.
    /// @return true if the object referenced by this IOInterface has set the "session" attribute
    /// @uptrace{SWS_CRYPT_10815}
    virtual bool IsObjectSession() const noexcept = 0;

    /// @brief Return volatility of the the underlying buffer of this IOInterface.
    ///        A "session" object can be stored to a "volatile" container only.
    ///        A content of a "volatile" container will be destroyed together with the interface instance.
    /// @return true if the container has a volatile nature (i.e. "temporary" or "in RAM") or false otherwise
    /// @uptrace{SWS_CRYPT_10814}
    virtual bool IsVolatile() const noexcept = 0;

    /// @brief Get whether the underlying KeySlot is valid.
    ///        An IOInterface is invalidated if the underlying resource has been modified after the IOInterface has been
    ///        opened.
    /// @return true if the underlying resource can be valid, false otherwise
    /// @uptrace{SWS_CRYPT_10823}
    virtual bool IsValid() const noexcept = 0;

    /// @brief Get whether the underlying KeySlot is writable -
    ///        if this IOInterface is linked to a VolatileTrustedContainer always return true.
    /// @return true if the underlying resource can be written
    /// @uptrace{SWS_CRYPT_10821}
    virtual bool IsWritable() const noexcept = 0;

    /// @brief Copy-assign another IOInterface to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30202}
    IOInterface& operator=(const IOInterface& other) = default;

    /// @brief Move-assign another IOInterface to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30203}
    IOInterface& operator=(IOInterface&& other) = default;
};

/// @brief Unique smart pointer of the interface.
/// @uptrace{SWS_CRYPT_10801}
using Uptr = std::unique_ptr<IOInterface>;

/// @brief Unique smart pointer of the constant interface.
/// @uptrace{SWS_CRYPT_10802}
using Uptrc = std::unique_ptr<const IOInterface>;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_IO_INTERFACE_H