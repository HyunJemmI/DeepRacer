// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H
#define ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H

#include "ara/core/result.h"
#include "ara/crypto/common/io_interface.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief A common interface for all cryptograhic objects recognizable by the Crypto Provider.
///        This interface (or any its derivative) represents a non-mutable (after completion) object
///        loadable to a temporary transformation context.
/// @uptrace{SWS_CRYPT_20500}
class CryptoObject
{
  public:
    /// @brief Unique identifier of this CryptoObject.
    /// @uptrace{SWS_CRYPT_20504}
    struct COIdentifier
    {
        /// @brief type of objext
        /// @uptrace{SWS_CRYPT_20506}
        CryptoObjectType mCOType;
        /// @brief object identifier
        /// @uptrace{SWS_CRYPT_20507}
        CryptoObjectUid mCouid;
    };

    /// @brief Unique smart pointer of the constant interface.
    /// @uptrace{SWS_CRYPT_20502}
    using Uptrc = std::unique_ptr<const CryptoObject>;

    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_20501}
    using Uptr = std::unique_ptr<CryptoObject>;

    /// @brief Destructor.
    /// @uptrace{SWS_CRYPT_20503}
    virtual ~CryptoObject() noexcept = default;

    /// @brief Downcast and move unique smart pointer from the generic CryptoObject interface to concrete derived
    /// object.
    /// @tparam ConcreteObject target type (derived from CryptoObject) for downcasting
    /// @param object unique smart pointer to the constant generic CryptoObject interface
    /// @return unique smart pointer to downcasted constant interface of specified derived type
    /// @uptrace{SWS_CRYPT_20518}
    template <class ConcreteObject>
    static ara::core::Result<typename ConcreteObject::Uptrc> Downcast(CryptoObject::Uptrc&& object) noexcept;

    /// @brief Return the CryptoPrimitivId of this CryptoObject.
    /// @return --
    /// @uptrace{SWS_CRYPT_20505}
    virtual CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept = 0;

    /// @brief Return the object’s COIdentifier, which includes the object’s type and UID.
    ///        An object that has no assigned COUID cannot be (securely) serialized / exported or saved to a
    ///        non-volatile storage. An object should not have a COUID if it is session and non-exportable
    ///        simultaneously A few related objects of different types can share a single COUID (e.g. private and public
    ///        keys), but a combination of COUID and object type must be unique always!
    /// @return the object’s COIdentifier including the object’s type and COUID (or an empty COUID, if this object is
    /// not identifiable).
    /// @uptrace{SWS_CRYPT_20514}
    virtual COIdentifier GetObjectId() const noexcept = 0;

    /// @brief Return actual size of the object’s payload. Returned value always must be less than or equal to
    ///        the maximum payload size expected for this primitive and object type, it is available via call:
    ///        MyProvider().GetPayloadStorageSize(GetObjectType(), GetPrimitiveId()).Value(); Returned value
    ///        does not take into account the object’s meta-information properties, but their size is fixed and
    ///        common for all crypto objects independently from their actual type. During an allocation of a
    ///        TrustedContainer, Crypto Providers (and Key Storage Providers) reserve space for an object’s
    ///        meta-information automatically, according to their implementation details.
    /// @return size in bytes of the object’s payload required for its storage
    /// @uptrace{SWS_CRYPT_20516}
    virtual std::size_t GetPayloadSize() const noexcept = 0;

    /// @brief Return the COIdentifier of the CryptoObject that this CryptoObject depends on.
    ///        For signatures objects this method must return a reference to correspondent signature verification public
    ///        key! Unambiguous identification of a CryptoObject requires both components: CryptoObjectUid and
    ///        CryptoObjectType.
    /// @return target COIdentifier of the existing dependence or CryptoObjectType::kUnknown and empty COUID,
    ///         if the current object does not depend on another CryptoObject
    /// @uptrace{SWS_CRYPT_20515}
    virtual COIdentifier HasDependence() const noexcept = 0;

    /// @brief Get the exportability attribute of the crypto object. An exportable object must have an assigned COUID
    /// (see GetObjectId()).
    /// @return true if the object is exportable (i.e. if it can be exported outside the trusted environment of the
    /// Crypto Provider)
    /// @uptrace{SWS_CRYPT_20513}
    virtual bool IsExportable() const noexcept = 0;

    /// @brief Return the "session" (or "temporary") attribute of the object. A temporary object cannot be saved
    ///        to a persistent storage location pointed to by an IOInterface! A temporary object will be
    ///        securely destroyed together with this interface instance! A non-session object must have an assigned
    ///        COUID (see GetObjectId()).
    /// @return true if the object is temporay (i.e. its life time is limited by the current session only)
    /// @uptrace{SWS_CRYPT_20512}
    virtual bool IsSession() const noexcept = 0;

    /// @brief Save itself to provided IOInterface A CryptoObject with property "session" cannot be saved in a KeySlot.
    /// @param container IOInterface representing underlying storage
    /// @return --
    /// @uptrace{SWS_CRYPT_20517}
    virtual ara::core::Result<void> Save(IOInterface& container) const noexcept = 0;

    /// @brief Copy-assign another CryptoObject to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30208}
    CryptoObject& operator=(const CryptoObject& other) = default;

    /// @brief Move-assign another CryptoObject to this instance.
    /// @param other the other instance
    /// @return *this, containing the contents of other
    /// @uptrace{SWS_CRYPT_30209}
    CryptoObject& operator=(CryptoObject&& other) = default;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_CRYOBJ_CRYPTO_OBJECT_H