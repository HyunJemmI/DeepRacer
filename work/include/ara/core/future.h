// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_FUTURE_H_
#define ARA_CORE_FUTURE_H_

#include <cassert>
#include <chrono>
#include <future>
#include <system_error>

#include "ara/core/core_error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"
#include "ara/core/future_error_domain.h"
#include "ara/core/internal/state.h"
#include "ara/core/result.h"

#if !defined(ATTR_NODISCARD)
#if __cplusplus >= 201703L
#define ar_attribute_nodiscard [[nodiscard]]
#else
#if defined(__GNUC__) || defined(__clang__)
#define ar_attribute_nodiscard __attribute__((warn_unused_result))
#else
#define ar_attribute_nodiscard /* nothing */
#endif
#endif
#define ATTR_NODISCARD ar_attribute_nodiscard
#endif

namespace ara
{
namespace core
{

/// Forward declaration
template <typename, typename>
class Promise;

/// @brief Specifies the state of a Future as returned by wait_for() and wait_until().
/// @uptrace{SWS_CORE_00361}
enum class future_status : std::uint8_t
{
    ready = 1,
    timeout,
};

inline std::ostream& operator<<(std::ostream& out, const FutureException& ex)
{
    return (out << "FutureException: " << ex.Error() << " (" << ex.what() << ")");
}

/// @brief Provides ara::core specific Future operations to collect the results of an asynchronous call.
/// @uptrace{SWS_CORE_00321}
template <typename T, typename E = ErrorCode>
class Future final
{
    using R = Result<T, E>;
    using Lock = std::unique_lock<std::mutex>;

  public:
    /// Alias type for T
    using ValueType = T;
    /// Alias type for the Promise type collaborating with this Future type
    using PromiseType = Promise<T, E>;
    /// @brief Default constructor
    /// @uptrace{SWS_CORE_00322}
    Future() noexcept = default;

    /// @brief Destructor for Future objects
    /// @uptrace{SWS_CORE_00333}
    ~Future()
    {
        Lock lock(mMutex);
        if (mExtraState) {
            mExtraState->SetCallback(nullptr);
        }
    }

    /// @uptrace{SWS_CORE_00334}
    Future(const Future&) = delete;
    /// @uptrace{SWS_CORE_00335}
    Future& operator=(const Future&) = delete;

    /// @uptrace{SWS_CORE_00323}
    Future(Future&& other) noexcept
        : mLock(other.mMutex),
          mDelegateFuture(std::move(other.mDelegateFuture)),
          mExtraState(std::move(other.mExtraState))
    {
        mLock.unlock();
    }

    /// @uptrace{SWS_CORE_00325}
    Future& operator=(Future&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mMutex, std::defer_lock);
            Lock rhsLock(other.mMutex, std::defer_lock);
            std::lock(lhsLock, rhsLock);
            mDelegateFuture = std::move(other.mDelegateFuture);
            mExtraState = std::move(other.extramExtraState_state_);
        }
        return *this;
    }

    /// @uptrace{SWS_CORE_00336}
    ATTR_NODISCARD R GetResult()
    {
#ifndef ARA_NO_EXCEPTIONS
        try {
            return mDelegateFuture.get();
        } catch (const std::future_error& ex) {
            const std::error_code& ec = ex.code();
            future_errc err;
            if (ec == std::future_errc::broken_promise) {
                err = future_errc::broken_promise;
            } else if (ec == std::future_errc::future_already_retrieved) {
                err = future_errc::future_already_retrieved;
            } else if (ec == std::future_errc::promise_already_satisfied) {
                err = future_errc::promise_already_satisfied;
            } else if (ec == std::future_errc::no_state) {
                err = future_errc::no_state;
            } else {
                // Should rather use a vendor/demonstrator-specific ErrorDomain here?
                return R::FromError(CoreErrc::kInvalidArgument);
            }
            R r = R::FromError(err);
            return r;
        }
#else
        // TODO: Somehow query the future object whether it contains an exception,
        // and if so, translate it into a Result<...>
        // This does not seem possible with C++14's std::future, because it lacks boost::future's
        // has_exception() method. Unfortunately, boost::future cannot be compiled with
        // -fno-exceptions, so that one is out as well.
        return mDelegateFuture.get();
#endif
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @brief Get the value.
    /// @returns value of type T
    /// @uptrace{SWS_CORE_00326}
    T get()
    {
        return GetResult().ValueOrThrow();
    }
#endif
    /// @brief Checks if the future is valid, i.e. if it has a shared state.
    /// @returns true if the future is usable, false otherwise
    /// @uptrace{SWS_CORE_00327}
    bool valid() const noexcept
    {
        return mDelegateFuture.valid();
    }

    /// @brief Waits for a value or an exception to be available.
    /// @uptrace{SWS_CORE_00328}
    void wait() const
    {
        mDelegateFuture.wait();
    }

    /// @brief Wait for the given period.
    /// @param timeout_duration maximal duration to wait for
    /// @returns status that indicates whether the timeout hit or if a value is available
    /// @uptrace{SWS_CORE_00329}
    template <typename Rep, typename Period>
    future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const
    {
        switch (mDelegateFuture.wait_for(timeout_duration)) {
            case std::future_status::ready:
                return future_status::ready;
            case std::future_status::timeout:
                return future_status::timeout;
            default:
                assert(!"this std::future_status should not occur in our setup");
        }
    }

    /// @brief Wait until the given time.
    /// @param deadline latest point in time to wait
    /// @returns status that indicates whether the time was reached or if a value is available
    /// @uptrace{SWS_CORE_00330}
    template <typename Clock, typename Duration>
    future_status wait_until(const std::chrono::time_point<Clock, Duration>& deadline) const
    {
        switch (mDelegateFuture.wait_until(deadline)) {
            case std::future_status::ready:
                return future_status::ready;
            case std::future_status::timeout:
                return future_status::timeout;
            default:
                assert(!"this std::future_status should not occur in our setup");
        }
    }

    /// @brief Register a function that gets called when the future becomes ready.
    /// @param func a Callable to register to get the Future result or an exception
    /// @uptrace{SWS_CORE_00331}
    template <typename F>
    void then(F&& func)
    {
        Lock lock(mMutex);
        mExtraState->SetCallback_NoLock(std::forward<F>(func));
        if (is_ready()) {
            mExtraState->FireCallback_NoLock();
        }
    }

    /// @brief True when the future contains either a result or an exception.
    /// @returns true if the Future contains data, false otherwise
    /// @uptrace{SWS_CORE_00332}
    bool is_ready() const
    {
        return std::future_status::ready == mDelegateFuture.wait_for(std::chrono::seconds::zero());
    }

  private:
    /// @brief Constructs a Future from a given std::future and a pointer to the extra state.
    /// @param delegate_future std::future instance
    /// @param extra_state state that is shared with the Promise
    Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state)
        : mDelegateFuture(std::move(delegate_future)), mExtraState(extra_state)
    {}

    std::mutex mMutex;
    Lock mLock;
    std::future<R> mDelegateFuture;
    internal::State::Ptr mExtraState;

    template <typename, typename>
    friend class Promise;
};

/// @brief Specialization of class Future for "void" values
/// @tparam E  the type of error
/// @uptrace{SWS_CORE_06221}
template <typename E>
class Future<void, E> final
{
    using R = Result<void, E>;

    using Lock = std::unique_lock<std::mutex>;

  public:
    /// Alias type for T
    using ValueType = void;
    /// Alias type for the Promise type collaborating with this Future type
    using PromiseType = Promise<void, E>;
    /// Alias type for the future_status type
    using Status = future_status;
    /// @uptrace{SWS_CORE_06222}
    Future() noexcept = default;

    /// @uptrace{SWS_CORE_06233}
    ~Future()
    {
        Lock lock(mMutex);

        if (mExtraState) {
            mExtraState->SetCallback(nullptr);
        }
    }

    /// @uptrace{SWS_CORE_06234}
    Future(const Future&) = delete;
    /// @uptrace{SWS_CORE_06235}
    Future& operator=(const Future&) = delete;

    /// @uptrace{SWS_CORE_06223}
    Future(Future&& other) noexcept
        : mLock(other.mMutex),
          mDelegateFuture(std::move(other.mDelegateFuture)),
          mExtraState(std::move(other.mExtraState))
    {
        mLock.unlock();
    }

    /// @uptrace{SWS_CORE_06225}
    Future& operator=(Future&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mMutex, std::defer_lock);
            Lock rhsLock(other.mMutex, std::defer_lock);
            std::lock(lhsLock, rhsLock);

            mDelegateFuture = std::move(other.mDelegateFuture);
            mExtraState = std::move(other.mExtraState);
        }
        return *this;
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @uptrace{SWS_CORE_06226}
    void get()
    {
        return GetResult().ValueOrThrow();
    }
#endif

    /// @uptrace{SWS_CORE_06236}
    ATTR_NODISCARD Result<void, E> GetResult() noexcept
    {
#ifndef ARA_NO_EXCEPTIONS
        try {
            return mDelegateFuture.get();
        } catch (const std::future_error& ex) {
            const std::error_code& ec = ex.code();
            future_errc err;
            if (ec == std::future_errc::broken_promise) {
                err = future_errc::broken_promise;
            } else if (ec == std::future_errc::future_already_retrieved) {
                err = future_errc::future_already_retrieved;
            } else if (ec == std::future_errc::promise_already_satisfied) {
                err = future_errc::promise_already_satisfied;
            } else if (ec == std::future_errc::no_state) {
                err = future_errc::no_state;
            } else {
                // Should rather use a vendor/demonstrator-specific ErrorDomain here?
                return R::FromError(CoreErrc::kInvalidArgument);
            }
            R r = R::FromError(err);
            return r;
        }
#else
        return mDelegateFuture.get();
#endif
    }

    /// @uptrace{SWS_CORE_06227}
    bool valid() const noexcept
    {
        return mDelegateFuture.valid();
    }

    /// @uptrace{SWS_CORE_06228}
    void wait() const
    {
        mDelegateFuture.wait();
    }

    /// @uptrace{SWS_CORE_06229}
    template <typename Rep, typename Period>
    future_status wait_for(const std::chrono::duration<Rep, Period>& timeoutDuration) const
    {
        switch (mDelegateFuture.wait_for(timeoutDuration)) {
            case std::future_status::ready:
                return future_status::ready;
            case std::future_status::timeout:
                return future_status::timeout;
            default:
                assert(!"this std::future_status should not occur in our setup");
        }
    }

    /// @uptrace{SWS_CORE_06230}
    template <typename Clock, typename Duration>
    future_status wait_until(const std::chrono::time_point<Clock, Duration>& deadline) const
    {
        switch (mDelegateFuture.wait_until(deadline)) {
            case std::future_status::ready:
                return future_status::ready;
            case std::future_status::timeout:
                return future_status::timeout;
            default:
                assert(!"this std::future_status should not occur in our setup");
        }
    }

    // @TODO not yet implemented, because the current general implementation above is nonsense anyway
    // @uptrace{SWS_CORE_06231}
    // template <typename F>
    // auto then(F&& func) -> Future<SEE_BELOW>;

    /// @uptrace{SWS_CORE_06232}
    bool is_ready() const
    {
        return std::future_status::ready == mDelegateFuture.wait_for(std::chrono::seconds::zero());
    }

  private:
    Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state)
        : mDelegateFuture(std::move(delegate_future)), mExtraState(extra_state)
    {}

    std::mutex mMutex;
    Lock mLock;
    std::future<R> mDelegateFuture;
    internal::State::Ptr mExtraState;

    template <typename, typename>
    friend class Promise;
};

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_FUTURE_H_
