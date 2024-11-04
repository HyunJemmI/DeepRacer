// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_COMMON_BASE_ID_TYPES_H
#define ARA_CRYPTO_COMMON_BASE_ID_TYPES_H

#include <cstdint>

#include "ara/core/vector.h"

namespace ara
{
namespace crypto
{

/// @brief A container type and constant bit-flags of allowed usages of a key or a secret seed object.
///        Only directly specified usages of a key are allowed, all other are prohibited!
///        Similar set of flags are defined for the usage restrictions of original key/seed and for a symmetric key or
///        seed that potentially can be derived from the original one. A symmetric key or secret seed can be derived
///        from the original one, only if it supports kAllowKeyAgreement or kAllowKeyDiversify or kAllow KeyDerivation!
/// @uptrace{SWS_CRYPT_10015}
using AllowedUsageFlags = std::uint32_t;

/// @brief Alias of a bytes’ vector template with customizable allocator.
/// @uptrace{SWS_CRYPT_10042}
/// @todo Alloc will be defined.
using ByteVector = ara::core::Vector<std::uint8_t>;

/// @brief Container type of the Crypto Algorithm Identifier.
/// @uptrace{SWS_CRYPT_10014}
using CryptoAlgId = std::uint64_t;

/// @brief Enumeration of all types of crypto objects, i.e. types of content that can be stored to a key slot.
/// @uptrace{SWS_CRYPT_10016}
enum class CryptoObjectType : std::uint32_t
{
    kUndefined = 0,     // Object type is currently not defined (empty container)
    kSymmetricKey = 1,  // cryp::SymmetricKey object
    kPrivateKey = 2,    // cryp::PrivateKey object
    kPublicKey = 3,     // cryp::PublicKey object
    kSignature = 4,     // cryp::Signature object (asymmetric digital signature or symmetric MAC/HMAC or hash digest)
    kSecretSeed = 5     // cryp::SecretSeed object. Note: the seed cannot have an associated crypto algorithm!
};

/// @brief Enumeration of all known Provider types.
/// @uptrace{SWS_CRYPT_10017}
enum class ProviderType : std::uint32_t
{
    kUndefinedProvider = 0,   // Undefined/Unknown Provider type (or applicable for the whole Crypto Stack)
    kCryptoProvider = 1,      // Cryptography Provider.
    kKeyStorageProvider = 2,  // Key Storage Provider.
    kX509Provider = 3         // X.509 Provider.
};

/// @brief Enumeration of cryptographic transformations.
/// @uptrace{SWS_CRYPT_10019}
enum class CryptoTransform : std::uint32_t
{
    kEncrypt = 1,      // encryption
    kDecrypt = 2,      // decryption
    kMacVerify = 3,    // MAC verification
    kMacGenerate = 4,  // MAC generation
    kWrap = 5,         // key wrapping
    kUnwrap = 6,       // key unwrapping
    kSigVerify = 7,    // signature verification
    kSigGenerate = 8   // signature generation
};

/// @brief Enumeration of key-slot types; currently only machine and applicaiton key-slots are defined.
/// @uptrace{SWS_CRYPT_10018}
enum class KeySlotType : std::uint32_t
{
    kMachine = 1,     // machine type key-slot - can be managed by application
    kApplication = 2  // application exclusive type key-slot
};

/// @brief Algorithm ID is undefined. Also this value may be used in meanings: Any or Default algorithm, None of
/// algorithms.
/// @uptrace{SWS_CRYPT_13000}
const CryptoAlgId kAlgIdUndefined = 0U;

/// @brief Any Algorithm ID is allowed.
/// @uptrace{SWS_CRYPT_13001}
const CryptoAlgId kAlgIdAny = kAlgIdUndefined;

/// @brief Default Algorithm ID (in current context/primitive).
/// @uptrace{SWS_CRYPT_13002}
const CryptoAlgId kAlgIdDefault = kAlgIdUndefined;

/// @brief None of Algorithm ID (i.e. an algorithm definition is not applicable).
/// @uptrace{SWS_CRYPT_13003}
const CryptoAlgId kAlgIdNone = kAlgIdUndefined;

/// @brief This group contains list of constant 1-bit values predefined for Allowed Usage flags.
///        The key/seed usage will be fully specified by a key slot prototype (the object can be used only after
///        reloading from the slot).
/// @uptrace{SWS_CRYPT_13100}
const AllowedUsageFlags kAllowPrototypedOnly = 0;

/// @brief The key/seed can be used for data encryption initialization (applicable to symmetric and asymmetric
/// algorithms).
/// @uptrace{SWS_CRYPT_13101}
const AllowedUsageFlags kAllowDataEncryption = 0x0001;

/// @brief The key/seed can be used for data decryption initialization (applicable to symmetric and asymmetric
/// algorithms).
/// @uptrace{SWS_CRYPT_13102}
const AllowedUsageFlags kAllowDataDecryption = 0x0002;

/// @brief The key/seed can be used for digital signature or MAC/HMAC production (applicable to symmetric and asymmetric
/// algorithms).
/// @uptrace{SWS_CRYPT_13103}
const AllowedUsageFlags kAllowSignature = 0x0004;

/// @brief The key/seed can be used for digital signature or MAC/HMAC verification (applicable to symmetric and
/// asymmetric algorithms).
/// @uptrace{SWS_CRYPT_13104}
const AllowedUsageFlags kAllowVerification = 0x0008;

/// @brief The seed or asymmetric key can be used for key-agreement protocol execution.
/// @uptrace{SWS_CRYPT_13105}
const AllowedUsageFlags kAllowKeyAgreement = 0x0010;

/// @brief The seed or symmetric key can be used for slave-keys diversification.
/// @uptrace{SWS_CRYPT_13106}
const AllowedUsageFlags kAllowKeyDiversify = 0x0020;

/// @brief The seed or symmetric key can be used for seeding of a RandomGeneratorCtx.
/// @uptrace{SWS_CRYPT_13107}
const AllowedUsageFlags kAllowRngInit = 0x0040;

/// @brief The object can be used as an input key material to KDF.
///        The seed or symmetric key can be used as a RestrictedUseObject for slave-keys derivation via a Key Derivation
///        Function (KDF).
/// @uptrace{SWS_CRYPT_13108}
const AllowedUsageFlags kAllowKdfMaterial = 0x0080;

/// @brief The key can be used as "transport" one for Key-Wrap or Encapsulate transformations (applicable to symmetric
/// and asymmetric keys).
/// @uptrace{SWS_CRYPT_13109}
const AllowedUsageFlags kAllowKeyExporting = 0x0100;

/// @brief The key can be used as "transport" one for Key-Unwrap or Decapsulate transformations (applicable to symmetric
/// and asymmetric keys).
/// @uptrace{SWS_CRYPT_13110}
const AllowedUsageFlags kAllowKeyImporting = 0x0200;

/// @brief The key can be used only for the mode directly specified by Key::AlgId.
/// @uptrace{SWS_CRYPT_13111}
const AllowedUsageFlags kAllowExactModeOnly = 0x8000;

/// @brief A derived seed or symmetric key can be used for data encryption.
/// @uptrace{SWS_CRYPT_13112}
const AllowedUsageFlags kAllowDerivedDataEncryption = kAllowDataEncryption << 16;

/// @brief A derived seed or symmetric key can be used for data decryption.
/// @uptrace{SWS_CRYPT_13113}
const AllowedUsageFlags kAllowDerivedDataDecryption = kAllowDataDecryption << 16;

/// @brief A derived seed or symmetric key can be used for MAC/HMAC production.
/// @uptrace{SWS_CRYPT_13114}
const AllowedUsageFlags kAllowDerivedSignature = kAllowSignature << 16;

/// @brief A derived seed or symmetric key can be used for MAC/HMAC verification.
/// @uptrace{SWS_CRYPT_13115}
const AllowedUsageFlags kAllowDerivedVerification = kAllowVerification << 16;

/// @brief A derived seed or symmetric key can be used for slave-keys diversification.
/// @uptrace{SWS_CRYPT_13116}
const AllowedUsageFlags kAllowDerivedKeyDiversify = kAllowKeyDiversify << 16;

/// @brief A derived seed or symmetric key can be used for seeding of a RandomGeneratorContext.
/// @uptrace{SWS_CRYPT_13117}
const AllowedUsageFlags kAllowDerivedRngInit = kAllowRngInit << 16;

/// @brief A derived seed or symmetric key can be used as a RestrictedUseObject for slave-keys derivation via a Key
/// Derivation Function (KDF).
/// @uptrace{SWS_CRYPT_13118}
const AllowedUsageFlags kAllowDerivedKdfMaterial = kAllowKdfMaterial << 16;

/// @brief A derived seed or symmetric key can be used as a "transport" one for Key-Wrap transformation.
/// @uptrace{SWS_CRYPT_13119}
const AllowedUsageFlags kAllowDerivedKeyExporting = kAllowKeyExporting << 16;

/// @brief A derived seed or symmetric key can be used as a "transport" one for Key-Unwrap transformation.
/// @uptrace{SWS_CRYPT_13120}
const AllowedUsageFlags kAllowDerivedKeyImporting = kAllowKeyImporting << 16;

/// @brief Restrict usage of derived objects to specified operation mode only.
///        A derived seed or symmetric key can be used only for the mode directly specified by Key::AlgId.
/// @uptrace{SWS_CRYPT_13121}
const AllowedUsageFlags kAllowDerivedExactModeOnly = kAllowExactModeOnly << 16;

/// @brief Allow usage of the object as a key material for KDF and any usage of derived objects.
///        The seed or symmetric key can be used as a RestrictedUseObject for a Key Derivation Function (KDF) and the
///        derived "slave" keys can be used without limitations.
/// @uptrace{SWS_CRYPT_13122}
const AllowedUsageFlags kAllowKdfMaterialAnyUsage =
    kAllowKdfMaterial | kAllowDerivedDataEncryption | kAllowDerivedDataDecryption | kAllowDerivedSignature |
    kAllowDerivedVerification | kAllowDerivedKeyDiversify | kAllowDerivedRngInit | kAllowDerivedKdfMaterial |
    kAllowDerivedKeyExporting | kAllowDerivedKeyImporting;

}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_COMMON_BASE_ID_TYPES_H