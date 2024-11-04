// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_ARRAY_H_
#define ARA_CORE_ARRAY_H_

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ara
{
namespace core
{

namespace detail
{

template <typename T, std::size_t N>
struct ArrayHolder final
{
    using Type = std::remove_cv_t<T>[N];

    static constexpr const T* ptr(const Type& data, std::size_t i) noexcept
    {
        return &data[i];
    }

    static constexpr T* ptr(Type& data, std::size_t i) noexcept
    {
        return &data[i];
    }
};

template <typename T>
struct ArrayHolder<T, 0> final
{
    struct Type final
    {};

    static constexpr const T* ptr(const Type&, std::size_t) noexcept
    {
        return nullptr;
    }

    static constexpr T* ptr(Type&, std::size_t) noexcept
    {
        return nullptr;
    }
};

namespace adl
{

using std::swap;

template <typename T>
struct is_nothrow_swappable : std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))>
{};

}  // namespace adl
}  // namespace detail

/// @uptrace{SWS_CORE_11200}
/// @uptrace{SWS_CORE_01201}
template <typename T, std::size_t N>
class Array final
{
  public:
    /// @uptrace{SWS_CORE_01210}
    using reference = T&;
    /// @uptrace{SWS_CORE_01211}
    using const_reference = const T&;
    /// @uptrace{SWS_CORE_01212}
    using iterator = T*;
    /// @uptrace{SWS_CORE_01213}
    using const_iterator = const T*;
    /// @uptrace{SWS_CORE_01214}
    using size_type = std::size_t;
    /// @uptrace{SWS_CORE_01215}
    using difference_type = std::ptrdiff_t;
    /// @uptrace{SWS_CORE_01216}
    using value_type = T;
    /// @uptrace{SWS_CORE_01217}
    using pointer = T*;
    /// @uptrace{SWS_CORE_01218}
    using const_pointer = const T*;
    /// @uptrace{SWS_CORE_01219}
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @uptrace{SWS_CORE_01220}
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    // Need to support N==0, for which we have to introduce an indirection.
    using ArrayHolder = detail::ArrayHolder<T, N>;
    typename ArrayHolder::Type mData;

    /// @uptrace{SWS_CORE_01241}
    void fill(const T& u)
    {
        std::fill(begin(), end(), u);
    }

    /// @uptrace{SWS_CORE_01242}
    void swap(Array<T, N>& other) noexcept(detail::adl::is_nothrow_swappable<T>::value)
    {
        std::swap_ranges(begin(), end(), other.begin());
    }

    /// @uptrace{SWS_CORE_01250}
    iterator begin() noexcept
    {
        return ArrayHolder::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01251}
    const_iterator begin() const noexcept
    {
        return ArrayHolder::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01252}
    iterator end() noexcept
    {
        return ArrayHolder::ptr(mData, N);
    }

    /// @uptrace{SWS_CORE_01253}
    const_iterator end() const noexcept
    {
        return ArrayHolder::ptr(mData, N);
    }

    /// @uptrace{SWS_CORE_01254}
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01255}
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01256}
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01257}
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01258}
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /// @uptrace{SWS_CORE_01259}
    const_iterator cend() const noexcept
    {
        return end();
    }

    /// @uptrace{SWS_CORE_01260}
    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /// @uptrace{SWS_CORE_01261}
    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    /// @uptrace{SWS_CORE_01262}
    constexpr size_type size() const noexcept
    {
        return N;
    }

    /// @uptrace{SWS_CORE_01263}
    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    /// @uptrace{SWS_CORE_01264}
    constexpr bool empty() const noexcept
    {
        return N == 0;
    }

    /// @uptrace{SWS_CORE_01265}
    reference operator[](size_type n)
    {
        return *ArrayHolder::ptr(mData, n);
    }

    /// @uptrace{SWS_CORE_01266}
    constexpr const_reference operator[](size_type n) const
    {
        return *ArrayHolder::ptr(mData, n);
    }

    /// @uptrace{SWS_CORE_01267}
    reference front()
    {
        return *ArrayHolder::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01268}
    constexpr const_reference front() const
    {
        return *ArrayHolder::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01269}
    reference back()
    {
        return *ArrayHolder::ptr(mData, N - 1);
    }

    /// @uptrace{SWS_CORE_01270}
    constexpr const_reference back() const
    {
        return *ArrayHolder::ptr(mData, N - 1);
    }

    /// @uptrace{SWS_CORE_01271}
    T* data() noexcept
    {
        return ArrayHolder::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01272}
    const T* data() const noexcept
    {
        return ArrayHolder::ptr(mData, 0);
    }
};

/// @uptrace{SWS_CORE_01290}
template <typename T, std::size_t N>
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/// @uptrace{SWS_CORE_01291}
template <typename T, std::size_t N>
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return !operator==(lhs, rhs);
}

/// @uptrace{SWS_CORE_01292}
template <typename T, std::size_t N>
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// @uptrace{SWS_CORE_01293}
template <typename T, std::size_t N>
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return rhs < lhs;
}

/// @uptrace{SWS_CORE_01294}
template <typename T, std::size_t N>
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return !(rhs < lhs);
}

/// @uptrace{SWS_CORE_01295}
template <typename T, std::size_t N>
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs)
{
    return !(lhs < rhs);
}

/// @uptrace{SWS_CORE_01296}
template <typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

/// @uptrace{SWS_CORE_01282}
template <std::size_t I, typename T, std::size_t N>
constexpr T& get(Array<T, N>& a) noexcept
{
    static_assert(I < N, "index out of bounds");
    return a[I];
}

/// @uptrace{SWS_CORE_01283}
template <std::size_t I, typename T, std::size_t N>
constexpr T&& get(Array<T, N>&& a) noexcept
{
    return std::move(get<I>(a));
}

/// @uptrace{SWS_CORE_01284}
template <std::size_t I, typename T, std::size_t N>
constexpr const T& get(const Array<T, N>& a) noexcept
{
    static_assert(I < N, "index out of bounds");
    return a[I];
}

}  // namespace core
}  // namespace ara

namespace std
{

/// @uptrace{SWS_CORE_01280}
template <typename T, size_t N>
struct tuple_size<ara::core::Array<T, N>> : integral_constant<size_t, N>
{};

/// @uptrace{SWS_CORE_01281}
template <size_t I, typename T, size_t N>
struct tuple_element<I, ara::core::Array<T, N>>
{
    /// @uptrace{SWS_CORE_01285}
    using type = T;
};

}  // namespace std

#endif  // ARA_CORE_ARRAY_H_
