// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_COM_SAMPLE_PTR_H_
#define ARA_COM_SAMPLE_PTR_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

#include "e2e/types.h"

namespace ara
{
namespace com
{

/// @uptrace{SWS_CM_00306}
template <typename T>
class SamplePtr
{
  private:
    class Deleter
    {
      public:
        using Function = std::function<void(T*)>;

        Deleter()
            : mF([](T* ptr) {
                  delete ptr;
              })
        {}

        explicit Deleter(const Function& f) : mF(f)
        {}

        void operator()(T* ptr) const
        {
            mF(ptr);
        }

      private:
        Function mF;
    };

  public:
    explicit constexpr SamplePtr(std::nullptr_t) noexcept : SamplePtr()
    {}

    explicit constexpr SamplePtr() noexcept : mDataPtr()
    {}

    explicit SamplePtr(T* ptr) : mDataPtr(ptr)
    {}

    explicit SamplePtr(T* ptr, const typename Deleter::Function& deleteFunction)
        : mDataPtr(ptr, Deleter(deleteFunction))
    {}

    SamplePtr(const SamplePtr<T>& r) noexcept = delete;
    SamplePtr& operator=(const SamplePtr<T>& r) noexcept = delete;

    SamplePtr(SamplePtr<T>&& r) noexcept : mDataPtr(std::move(r.mDataPtr))
    {}

    SamplePtr& operator=(std::nullptr_t) noexcept
    {
        mDataPtr = nullptr;
        return *this;
    }

    SamplePtr& operator=(SamplePtr<T>&& r) noexcept
    {
        mDataPtr = std::move(r.mDataPtr);
        return *this;
    }

    void Swap(SamplePtr& other) noexcept
    {
        using std::swap;
        swap(mDataPtr, other.mDataPtr);
    }

    void Reset(std::nullptr_t)
    {
        mDataPtr.reset();
    }

    explicit operator bool() const noexcept
    {
        return bool(mDataPtr);
    }

    T& operator*() const noexcept
    {
        return mDataPtr.operator*();
    }

    T* operator->() const noexcept
    {
        return mDataPtr.operator->();
    }

    T* Get() const noexcept
    {
        return mDataPtr.get();
    }

    /// @uptrace{SWS_CM_90420}
    ara::com::e2e::ProfileCheckStatus GetProfileCheckStatus() const noexcept
    {
        return mProfileCheckStatus;
    }

    void SetProfileCheckStatus(ara::com::e2e::ProfileCheckStatus status)
    {
        mProfileCheckStatus = status;
    }

  private:
    std::unique_ptr<T, Deleter> mDataPtr;
    ara::com::e2e::ProfileCheckStatus mProfileCheckStatus;
};

template <typename T, typename... Args>
SamplePtr<T> make_sample_ptr(Args&&... args)
{
    return SamplePtr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace com
}  // namespace ara

#endif  // ARA_COM_SAMPLE_PTR_H_