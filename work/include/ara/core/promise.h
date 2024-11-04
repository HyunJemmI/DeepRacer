// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_PROMISE_H_
#define ARA_CORE_PROMISE_H_

#include <exception>
#include <future>
#include <mutex>
#include <system_error>

#include "ara/core/error_code.h"
#include "ara/core/future.h"
#include "ara/core/internal/state.h"
#include "ara/core/result.h"

namespace ara
{
namespace core
{

/// @brief ara::core specific variant of std::promise class
/// @uptrace{SWS_CORE_00340}
template <typename T, typename E = ErrorCode>
class Promise final
{
    using R = Result<T, E>;
    using Lock = std::unique_lock<std::mutex>;

  public:
    /// @uptrace{SWS_CORE_00341}
    Promise() : mExtraState(std::make_shared<internal::State>())
    {}

    /// @uptrace{SWS_CORE_00349}
    ~Promise()
    {}

    /// @uptrace{SWS_CORE_00350}
    Promise(const Promise&) = delete;
    /// @uptrace{SWS_CORE_00351}
    Promise& operator=(const Promise&) = delete;

    /// @uptrace{SWS_CORE_00342}
    Promise(Promise&& other) noexcept
        : mLock(other.mMutex),
          mDelegatePromise(std::move(other.mDelegatePromise)),
          mExtraState(std::move(other.mExtraState))
    {
        mLock.unlock();
    }

    /// @uptrace{SWS_CORE_00343}
    Promise& operator=(Promise&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mMutex, std::defer_lock);
            Lock rhsLock(other.mMutex, std::defer_lock);
            std::lock(lhsLock, rhsLock);

            mDelegatePromise = std::move(other.mDelegatePromise);
            mExtraState = std::move(other.mExtraState);
        }
        return *this;
    }

    /// @uptrace{SWS_CORE_00352}
    void swap(Promise& other) noexcept
    {
        Lock lhsLock(mMutex, std::defer_lock);
        Lock rhsLock(other.mMutex, std::defer_lock);
        std::lock(lhsLock, rhsLock);

        using std::swap;
        swap(mDelegatePromise, other.mDelegatePromise);
        swap(mExtraState, other.mExtraState);
    }

    /// @brief Returns an associated Future for type T.
    /// @returns a Future for type T
    /// @uptrace{SWS_CORE_00344}
    Future<T, E> get_future()
    {
        return Future<T, E>(mDelegatePromise.get_future(), mExtraState);
    }

    /// @uptrace{SWS_CORE_00353}
    void SetError(E&& err)
    {
        R r = R::FromError(std::move(err));
        SetResult(std::move(r));
    }

    /// @uptrace{SWS_CORE_00354}
    void SetError(const E& err)
    {
        R r = R::FromError(err);
        SetResult(r);
    }

    /// @uptrace{SWS_CORE_00355}
    void SetResult(const Result<T, E>& result)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_value(result);
        mExtraState->FireCallback_NoLock();
    }

    /// @uptrace{SWS_CORE_00356}
    void SetResult(Result<T, E>&& result)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_value(std::move(result));
        mExtraState->FireCallback_NoLock();
    }

    /// @brief Sets an exception.
    /// @param p exception_ptr to set
    /// @note This method is DEPRECATED. The exception is defined by the error code.
    void set_exception(std::exception_ptr p)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_exception(p);
        mExtraState->FireCallback_NoLock();
    }

    /// @brief Move the result into the future.
    /// @param value the value to store
    /// @uptrace{SWS_CORE_00346}
    void set_value(T&& value)
    {
        R r = std::move(value);
        SetResult(std::move(r));
    }

    /// @brief Copy result into the future.
    /// @param value the value to store
    /// @uptrace{SWS_CORE_00345}
    void set_value(const T& value)
    {
        R r = value;
        SetResult(r);
    }

  private:
    std::mutex mMutex;
    Lock mLock;
    std::promise<R> mDelegatePromise;
    internal::State::Ptr mExtraState;
};

/// @brief Specialization of class Promise for "void" values
/// @tparam E  the type of error
/// @uptrace{SWS_CORE_06340}
template <typename E>
class Promise<void, E> final
{
    using R = Result<void, E>;
    using Lock = std::unique_lock<std::mutex>;

  public:
    /// @uptrace{SWS_CORE_06341}
    Promise() : mExtraState(std::make_shared<internal::State>())
    {}

    /// @uptrace{SWS_CORE_06350}
    Promise(const Promise&) = delete;

    /// @uptrace{SWS_CORE_06342}
    Promise(Promise&& other) noexcept
        : mLock(other.mMutex),
          mDelegatePromise(std::move(other.mDelegatePromise)),
          mExtraState(std::move(other.mExtraState))
    {
        mLock.unlock();
    }

    /// @uptrace{SWS_CORE_06349}
    ~Promise()
    {}

    /// @uptrace{SWS_CORE_06351}
    Promise& operator=(const Promise&) = delete;

    /// @uptrace{SWS_CORE_06343}
    Promise& operator=(Promise&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mMutex, std::defer_lock);
            Lock rhsLock(other.mMutex, std::defer_lock);
            std::lock(lhsLock, rhsLock);

            mDelegatePromise = std::move(other.mDelegatePromise);
            mExtraState = std::move(other.mExtraState);
        }
        return *this;
    }

    /// @uptrace{SWS_CORE_06352}
    void swap(Promise& other) noexcept
    {
        Lock lhsLock(mMutex, std::defer_lock);
        Lock rhsLock(other.mMutex, std::defer_lock);
        std::lock(lhsLock, rhsLock);

        using std::swap;
        swap(mDelegatePromise, other.mDelegatePromise);
        swap(mExtraState, other.mExtraState);
    }

    /// @uptrace{SWS_CORE_06344}
    Future<void, E> get_future()
    {
        return Future<void, E>(mDelegatePromise.get_future(), mExtraState);
    }

    /// @uptrace{SWS_CORE_06345}
    void set_value()
    {
        R r = R::FromValue();
        SetResult(std::move(r));
    }

    /// @uptrace{SWS_CORE_06353}
    void SetError(E&& err)
    {
        R r = R::FromError(std::move(err));
        SetResult(std::move(r));
    }

    /// @uptrace{SWS_CORE_06354}
    void SetError(const E& err)
    {
        R r = R::FromError(err);
        SetResult(r);
    }

    /// @uptrace{SWS_CORE_06355}
    void SetResult(const Result<void, E>& result)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_value(result);
        mExtraState->FireCallback_NoLock();
    }

    /// @uptrace{SWS_CORE_06356}
    void SetResult(Result<void, E>&& result)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_value(std::move(result));
        mExtraState->FireCallback_NoLock();
    }

    // unspecified, remove this!
    void set_exception(std::exception_ptr p)
    {
        Lock lock(mMutex);
        mDelegatePromise.set_exception(p);
        mExtraState->FireCallback_NoLock();
    }

  private:
    std::mutex mMutex;
    Lock mLock;
    std::promise<R> mDelegatePromise;
    internal::State::Ptr mExtraState;
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_PROMISE_H_
