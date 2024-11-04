// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H
#define ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H

#include <memory>

#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
namespace crypto
{
namespace cryp
{

/// @brief Generalized Stream Cipher Context interface (it covers all modes of operation).
/// @uptrace{SWS_CRYPT_23600}
class StreamCipherCtx : public CryptoContext
{
  public:
    /// @brief Unique smart pointer of the interface.
    /// @uptrace{SWS_CRYPT_23601}
    using Uptr = std::unique_ptr<StreamCipherCtx>;

    /// @brief Count number of bytes now kept in the context cache.
    ///        In block-wise modes if an application has supplied input data chunks with incomplete last block
    ///        then the context saves the rest part of the last (incomplete) block to internal "cache" memory
    ///        and wait a next call for additional input to complete this block.
    /// @return number of bytes now kept in the context cache
    /// @uptrace{SWS_CRYPT_23620}
    virtual std::size_t CountBytesInCache() const noexcept = 0;

    /// @brief Estimate maximal number of input bytes that may be processed for filling of an output buffer without
    /// overflow.
    /// @param outputCapacity capacity of the output buffer
    /// @return maximum number of input bytes
    /// @uptrace{SWS_CRYPT_23621}
    std::size_t EstimateMaxInputSize(std::size_t outputCapacity) const noexcept;

    /// @brief Estimate minimal required capacity of the output buffer, which is enough for saving a result of input
    /// data processing.
    /// @param inputSize size of input data
    /// @param isFinal flag that indicates processing of the last data chunk (if true)
    /// @return required capacity of the output buffer (in bytes)
    /// @uptrace{SWS_CRYPT_23622}
    std::size_t EstimateRequiredCapacity(std::size_t inputSize, bool isFinal = false) const noexcept;

    /// @brief Processe the final part of message (that may be not aligned to the block-size boundary).
    ///        If (IsBytewiseMode() == false) then it must be: bs = GetBlockSize(), out.size() >= (((in.size() + bs *
    ///        ((CryptoTransform::kEncrypt == GetTransformation().Value()) ? 2 : 1) 1) / bs) * bs) If (IsBytewiseMode()
    ///        == true) then it must be: out.size() >= in.size() The input and output buffers must not intersect! Usage
    ///        of this method is mandatory for processing of the last data chunk in block-wise modes! This method may be
    ///        used for processing of a whole message in a single call (in any mode)! in an input data buffer an output
    ///        data buffer SecurityErrorDomain::kInsufficientCapacity if capacity of the output buffer is not enough
    ///        SecurityErrorDomain::kInOutBuffersIntersect if the input and output buffers intersect
    ///        SecurityErrorDomain::kProcessingNotStarted if data processing was not started by a call ofthe Start()
    ///        method
    /// @param in an input data buffer
    /// @return an output data buffer
    /// @uptrace{SWS_CRYPT_23618}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> FinishBytes(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Processe the final part of message (that may be not aligned to the block-size boundary).
    ///        This method sets the size of the output container according to actually saved value.
    ///        If (IsBytewiseMode() == false) then it must be: bs = GetBlockSize(), out.capacity() >= (((in.size() + bs
    ///        * ((CryptoTransform::kEncrypt == GetTransformation.Value()) ? 2 : 1) - 1) / bs) * bs) If
    ///        (IsBytewiseMode() == true) then it must be: out.capacity() >= in.size() Usage of this method is mandatory
    ///        for processing of the last data chunk in block-wise modes! This method may be used for processing of a
    ///        whole message in a single call (in any mode)!
    /// @uptrace{SWS_CRYPT_23619}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> FinishBytes(ReadOnlyMemRegion in) noexcept;

    /// @brief Get BlockService instance.
    /// @return --
    /// @uptrace{SWS_CRYPT_23602}
    virtual BlockService::Uptr GetBlockService() const noexcept = 0;

    /// @brief Check the operation mode for the bytewise property.
    /// @return true if the mode can process messages the byte-by-byte (without padding up to the block boundary) and
    ///         false if only the block-by-block (only full blocks can be processed, the padding is mandatory)
    /// @uptrace{SWS_CRYPT_23611}
    virtual bool IsBytewiseMode() const noexcept = 0;

    /// @brief Get the kind of transformation configured for this context: kEncrypt or kDecrypt.
    /// @return CryptoTransform
    /// @uptrace{SWS_CRYPT_23624}
    virtual ara::core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

    /// @brief Check if the seek operation is supported in the current mode.
    /// @return true the seek operation is supported in the current mode and false otherwise
    /// @uptrace{SWS_CRYPT_23612}
    virtual bool IsSeekableMode() const noexcept = 0;

    /// @brief Processe initial parts of message aligned to the block-size boundary.
    ///        It is a copy-optimized method that doesn’t use the internal cache buffer! It can be used only before
    ///        processing of any non-aligned to the block-size boundary data. Pointers to the input and output buffers
    ///        must be aligned to the block-size boundary! The input and output buffers may completely coincide, but
    ///        they must not partially intersect!
    /// @param in an input data buffer
    /// @return an output data buffer
    /// @uptrace{SWS_CRYPT_23614}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Processe initial parts of message aligned to the block-size boundary.
    ///        It is a copy-optimized method that doesn’t use internal cache buffer! It can be used up to first
    ///        non-block aligned data processing. Pointer to the input-output buffer must be aligned to the block-size
    ///        boundary!
    /// @param inOut an input and output data buffer, i.e. the whole buffer should be updated
    /// @return an input and output data buffer, i.e. the whole buffer should be updated
    /// @uptrace{SWS_CRYPT_23615}
    virtual ara::core::Result<void> ProcessBlocks(ReadWriteMemRegion inOut) noexcept = 0;

    /// @brief Processe a non-final part of message (that is not aligned to the block-size boundary).
    ///        If (IsBytewiseMode() == false) then it must be: bs = GetBlockSize(), out.size() >= (((in.size() + bs - 1)
    ///        / bs) * bs) If (IsBytewiseMode() == true) then it must be: out.size() >= in.size() The input and output
    ///        buffers must not intersect! This method is "copy inefficient", therefore it should be used only in
    ///        conditions when an application cannot control the chunking of the original message!
    /// @param in an input data buffer
    /// @return an output data buffer
    /// @uptrace{SWS_CRYPT_23616}
    virtual ara::core::Result<ara::core::Vector<ara::core::Byte>> ProcessBytes(ReadOnlyMemRegion in) noexcept = 0;

    /// @brief Processes a non-final part of message (that is not aligned to the block-size boundary).
    ///        This method sets size of the output container according to actually saved value.
    ///        If (IsBytewiseMode() == false) then it must be: bs = GetBlockSize(), out.capacity() >= (((in.size() + bs
    ///        - 1) / bs) * bs) If (IsBytewiseMode() == true) then it must be: out.capacity() >= in.size() This method
    ///        is "copy inefficient", therefore it should be used only in conditions when an application cannot control
    ///        the chunking of the original message! The input buffer must not point inside the output container!
    /// @param in an input data buffer
    /// @return a managed container for the output data
    /// @uptrace{SWS_CRYPT_23617}
    template <typename Alloc = <implementation - defined>>
    ara::core::Result<ByteVector<Alloc>> ProcessBytes(ReadOnlyMemRegion in) noexcept;

    /// @brief Clear the crypto context.
    /// @return --
    /// @uptrace{SWS_CRYPT_23627}
    virtual ara::core::Result<void> Reset() noexcept = 0;

    /// @brief Set the position of the next byte within the stream of the encryption/decryption gamma.
    /// @param offset the offset value in bytes, relative to begin or current position in the gamma stream
    /// @param fromBegin the starting point for positioning within the stream: from begin (if true) or from current
    /// position (if false)
    /// @return --
    /// @uptrace{SWS_CRYPT_23613}
    virtual ara::core::Result<void> Seek(std::int64_t offset, bool fromBegin = true) noexcept = 0;

    /// @brief Set (deploy) a key to the stream chiper algorithm context.
    /// @param key the source key object
    /// @param transform --
    /// @return --
    /// @uptrace{SWS_CRYPT_23623}
    virtual ara::core::Result<void> SetKey(const SymmetricKey& key,
                                           CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv an optional Initialization Vector (IV) or "nonce" value
    /// @return --
    /// @uptrace{SWS_CRYPT_23625}
    virtual ara::core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

    /// @brief Initialize the context for a new data stream processing or generation (depending from the primitive).
    ///        If IV size is greater than maximally supported by the algorithm then an implementation may use the
    ///        leading bytes only from the sequence.
    /// @param iv the Initialization Vector (IV) or "nonce" object
    /// @return --
    /// @uptrace{SWS_CRYPT_23626}
    virtual ara::core::Result<void> Start(const SecretSeed& iv) noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_CRYP_STREAM_CIPHER_CTX_H
