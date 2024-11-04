// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_KEY_VALUE_STORAGE_H_
#define ARA_PER_KEY_VALUE_STORAGE_H_

#include <memory>
#include <utility>


#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "ara/per/per_error_domain.h"
#include "ara/per/shared_handle.h"

#include "para/serialization/serializer.h"

namespace ara
{
namespace per
{

// @uptrace{SWS_PER_00339}
class KeyValueStorage final
{

  public:

    explicit KeyValueStorage(const ara::core::InstanceSpecifier& kvs);

    // @uptrace{SWS_PER_00322}
    // KeyValueStorage(KeyValueStorage &&kvs) noexcept = default;
    KeyValueStorage(KeyValueStorage&& kvs) noexcept;

    // @uptrace{SWS_PER_00323}
    // KeyValueStorage& operator= (KeyValueStorage &&kvs) &noexcept = default;
    KeyValueStorage& operator=(KeyValueStorage&& kvs) & noexcept;

    // @uptrace{SWS_PER_00324}
    KeyValueStorage(const KeyValueStorage&) = delete;

    // @uptrace{SWS_PER_00325}
    KeyValueStorage& operator=(const KeyValueStorage&) = delete;

    // @uptrace{SWS_PER_00050}
    ~KeyValueStorage() noexcept;

    ara::core::Result<void> Init(const std::string& kvsInstanceName);

    // @uptrace{SWS_PER_00042}
    ara::core::Result<ara::core::Vector<ara::core::String>> GetAllKeys() const noexcept;

    // @uptrace{SWS_PER_00043}
    ara::core::Result<bool> KeyExists(ara::core::StringView key) const noexcept;

    // @uptrace{SWS_PER_00332}
    template <class T>
    ara::core::Result<T> GetValue(ara::core::StringView key) const noexcept
    {
        para::serializer::Serializer serializer{};
        ara::core::String valueTypeName = serializer.getTypeName<T>();

        auto ptrBufferResult = GetValueBuffer(key, valueTypeName);
        if (!ptrBufferResult.HasValue()) {
            return ara::core::Result<T>{ptrBufferResult.Error()};
        }

        const auto *ptrBuffer = ptrBufferResult.Value();
        para::serializer::Deserializer deserializer{*ptrBuffer};

        T result;
        deserializer.read<T>(result);

        return ara::core::Result<T>(result);
    }

    // @uptrace{SWS_PER_00046}
    template <class T>
    ara::core::Result<void> SetValue(ara::core::StringView key, const T& value) noexcept
    {
        // serialize data
        para::serializer::Serializer serializer{};

        serializer.write<T>(value);
        auto dataBuffer = serializer.ensure();
        ara::core::String valueTypeName = serializer.getTypeName<T>();

        return SetValueBuffer(key, valueTypeName, std::move(dataBuffer));
    }

    // @uptrace{SWS_PER_00047}
    ara::core::Result<void> RemoveKey(ara::core::StringView key) noexcept;

    // @uptrace{SWS_PER_00427}
    ara::core::Result<void> RecoverKey(ara::core::StringView key) noexcept;

    // @uptrace{SWS_PER_00426}
    ara::core::Result<void> ResetKey(ara::core::StringView key) noexcept;

    // @uptrace{SWS_PER_00048}
    ara::core::Result<void> RemoveAllKeys() noexcept;

    // @uptrace{SWS_PER_00049}
    ara::core::Result<void> SyncToStorage() noexcept;

    // @uptrace{SWS_PER_00365}
    ara::core::Result<void> DiscardPendingChanges() noexcept;

    bool IsInitialized() const;

  private:

    ara::core::Result<const std::vector<std::uint8_t>*> GetValueBuffer(ara::core::StringView key, const ara::core::String& valueTypeName) const noexcept;

    ara::core::Result<void> SetValueBuffer(ara::core::StringView key, const ara::core::String& valueTypeName, std::vector<std::uint8_t>&& buffer);



  private:

    // Forward declaration of the implementation class
    class Impl;
    // The implementation class
    std::unique_ptr<Impl> mImpl;

};

// @uptrace{SWS_PER_00052}
ara::core::Result<SharedHandle<KeyValueStorage>> OpenKeyValueStorage(const ara::core::InstanceSpecifier& kvs) noexcept;

// @uptrace{SWS_PER_00333}
ara::core::Result<void> RecoverKeyValueStorage(const ara::core::InstanceSpecifier& kvs) noexcept;

// @uptrace{SWS_PER_00334}
ara::core::Result<void> ResetKeyValueStorage(const ara::core::InstanceSpecifier& kvs) noexcept;

// @uptrace{SWS_PER_00405}
ara::core::Result<std::uint64_t> GetCurrentKeyValueStorageSize(const ara::core::InstanceSpecifier& kvs) noexcept;

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_KEY_VALUE_STORAGE_H_