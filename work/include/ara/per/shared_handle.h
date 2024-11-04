// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_PER_SHARED_HANDLE_H_
#define ARA_PER_SHARED_HANDLE_H_

#include <memory>

namespace ara
{
namespace per
{

//[SWS_PER_00362]
template <typename T>
class SharedHandle final
{
  public:
    SharedHandle(std::shared_ptr<T> ptr) : m_ptr(ptr)
    {}

    //[SWS_PER_00367]
    SharedHandle(SharedHandle&& sh) noexcept = default;

    //[SWS_PER_00369]
    SharedHandle(const SharedHandle& sh) = default;

    //[SWS_PER_00368]
    SharedHandle& operator=(SharedHandle&& sh) & = default;

    //[SWS_PER_00370]
    SharedHandle& operator=(const SharedHandle& sh) & = default;

    //[SWS_PER_00398]
    explicit operator bool() const noexcept
    {
        return (m_ptr.use_count() > 0);
    }

    //[SWS_PER_00363]
    T* operator->() noexcept
    {
        return m_ptr.get();
    }

    //[SWS_PER_00364]
    const T* operator->() const noexcept
    {
        return m_ptr.get();
    }

    //[SWS_PER_00402]
    T& operator*() noexcept
    {
        return *m_ptr.get();
    }

    //[SWS_PER_00403]
    const T& operator*() const noexcept
    {
        return *m_ptr.get();
    }

  private:
    std::shared_ptr<T> m_ptr;
};

}  // namespace per
}  // namespace ara

#endif  // ARA_PER_SHARED_HANDLE_H_