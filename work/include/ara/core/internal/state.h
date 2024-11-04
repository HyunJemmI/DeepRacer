// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_INTERNAL_STATE_H
#define ARA_CORE_INTERNAL_STATE_H

#include <memory>
#include <mutex>

namespace ara
{
namespace core
{
namespace internal
{
/// @brief Class State maintains the action (Set, Get, Fire or Execute) on callback
class State
{
  public:
    typedef std::shared_ptr<State> Ptr;

    State()
    {}

    ~State()
    {}

    /// @brief Executes the stored callback.
    void FireCallback()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (HasCallback_NoLock()) {
            mCallback();
        }
    }

    /// @brief Executes the stored callback without mutex lock.
    void FireCallback_NoLock()
    {
        if (HasCallback_NoLock()) {
            mCallback();
        }
    }

    /// @brief Sets the callback. any callback that was set before is removed and replaced by the new callback.
    /// @param callback The callback to be set.
    template <typename F>
    void SetCallback(F&& callback)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        SetCallback_NoLock(std::forward<F>(callback));
    }

    /// @brief Returns if a callback was set previously.
    /// @return true if a callback is set, false otherwise.
    bool HasCallback()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        return HasCallback_NoLock();
    }

    /// @brief Sets the callback. any callback that was set before is removed and replaced by the new callback.
    /// @param callback The callback to be set.
    template <typename F>
    void SetCallback_NoLock(F&& callback)
    {
        mCallback = std::forward<F>(callback);
    }

  private:
    /// @brief Returns if a callback was set previously.
    /// @note This function assumes a mutex lock is already set
    /// @return true if a callback is set, false otherwise.
    bool HasCallback_NoLock() const
    {
        return static_cast<bool>(mCallback);
    }

    std::function<void(void)> mCallback;
    std::mutex mMutex;
};

}  // namespace internal
}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_INTERNAL_STATE_H
