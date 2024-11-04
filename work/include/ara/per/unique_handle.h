// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_UNIQUE_HANDLE_H_
#define ARA_PER_UNIQUE_HANDLE_H_

#include <memory>

namespace ara
{
namespace per
{
//[SWS_PER_00359]
template <typename T>
class UniqueHandle final
{
  public:
    UniqueHandle(std::unique_ptr<T>&& ptr) : m_ptr(std::move(ptr)){};
    //[SWS_PER_00371]
    UniqueHandle(UniqueHandle&& uh) noexcept = default;

    //[SWS_PER_00373]
    UniqueHandle(const UniqueHandle&) = delete;

    //[SWS_PER_00372]
    UniqueHandle& operator=(UniqueHandle&& uh) & noexcept = default;

    //[SWS_PER_00374]
    UniqueHandle& operator=(const UniqueHandle&) = delete;

    //[SWS_PER_00399]
    explicit operator bool() const noexcept
    {
        return !!m_ptr;
    }

    //[SWS_PER_00360]
    T* operator->() noexcept
    {
        return m_ptr.get();
    }

    //[SWS_PER_00361]
    const T* operator->() const noexcept
    {
        return m_ptr.get();
    }

    //[SWS_PER_00400]
    T& operator*() noexcept
    {
        return *m_ptr.get();
    }

    //[SWS_PER_00401]
    const T& operator*() const noexcept
    {
        return *m_ptr.get();
    }

  private:
    std::unique_ptr<T> m_ptr;
};

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_UNIQUE_HANDLE_H_