// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_SPAN_H_
#define ARA_CORE_SPAN_H_

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>

#include "ara/core/array.h"
#include "ara/core/utility.h"

namespace ara
{
namespace core
{

namespace
{
constexpr static bool DEBUG_SPAN = false;  // false or true

template <typename... Args>
static inline void xtrace(const Args&... args)
{
    if (DEBUG_SPAN) {
        using expander = int[];
        static_cast<void>(expander{0, (static_cast<void>(std::cerr << args), 0)...});
    }
}
}  // namespace

/// @uptrace{SWS_CORE_01901}
constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

template <typename T, std::size_t Extent>
class Span;

namespace detail
{

// Re-implementation of std::remove_cvref from C++20.
template <typename T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

// Re-implementation of std::remove_cvref_t from C++20.
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename T>
struct is_ara_core_span_checker : public std::false_type
{};

template <typename T, std::size_t Extent>
struct is_ara_core_span_checker<core::Span<T, Extent>> : public std::true_type
{};

template <typename T>
struct is_ara_core_span : public is_ara_core_span_checker<remove_cvref_t<T>>
{};

template <typename T>
struct is_ara_core_array_checker : public std::false_type
{};

template <typename T, std::size_t N>
struct is_ara_core_array_checker<core::Array<T, N>> : public std::true_type
{};

template <typename T>
struct is_ara_core_array : public is_ara_core_array_checker<remove_cvref_t<T>>
{};

template <typename T>
struct is_std_array_checker : public std::false_type
{};

template <typename T, std::size_t N>
struct is_std_array_checker<std::array<T, N>> : public std::true_type
{};

template <typename T>
struct is_std_array : public is_std_array_checker<remove_cvref_t<T>>
{};

template <typename, typename = size_t>
struct is_complete : std::false_type
{};

template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type
{};

// Re-implementation of std::void_t from C++20.
template <typename...>
using void_t = void;

template <typename, typename, typename, typename = void>
struct is_suitable_container : public std::false_type
{};

template <typename C, typename ArgT, typename ElementType>
struct is_suitable_container<
    C, ArgT, ElementType,
    void_t<                                                      //
        std::enable_if_t<!detail::is_ara_core_span<C>::value>,   //
        std::enable_if_t<!detail::is_ara_core_array<C>::value>,  //
        std::enable_if_t<!detail::is_std_array<C>::value>,       //
        std::enable_if_t<!std::is_array<C>::value>,              //
        decltype(core::data(std::declval<ArgT>())),              //
        decltype(core::size(std::declval<ArgT>())),              //
        std::enable_if_t<std::is_convertible<std::remove_pointer_t<decltype(core::data(std::declval<ArgT>()))> (*)[],
                                             ElementType (*)[]>::value>  //
        >                                                                //
    > : public std::true_type
{};

}  // namespace detail

/// @uptrace{SWS_CORE_01900}
template <typename T, std::size_t Extent = dynamic_extent>
class Span
{
    /// @uptrace{SWS_CORE_01900}
    static_assert(std::is_object<T>::value, "Span cannot contain reference or void types");
    /// @uptrace{SWS_CORE_01900}
    static_assert(!std::is_abstract<T>::value, "Span cannot contain abstract types");
    /// @uptrace{SWS_CORE_01900}
    static_assert(detail::is_complete<T>::value, "Span cannot contain incomplete types");

    template <typename U>
    using type_of_data = decltype(ara::core::data(std::declval<U>()));

  public:
    /// @uptrace{SWS_CORE_01911}
    using element_type = T;
    /// @uptrace{SWS_CORE_01912}
    using value_type = typename std::remove_cv<element_type>::type;
    /// @uptrace{SWS_CORE_01921}
    using size_type = std::size_t;
    /// @uptrace{SWS_CORE_01914}
    using difference_type = std::ptrdiff_t;
    /// @uptrace{SWS_CORE_01915}
    using pointer = element_type*;
    /// @uptrace{SWS_CORE_01922}
    using const_pointer = const element_type*;
    /// @uptrace{SWS_CORE_01916}
    using reference = element_type&;
    /// @uptrace{SWS_CORE_01923}
    using const_reference = const element_type&;
    /// @uptrace{SWS_CORE_01917}
    using iterator = element_type*;
    /// @uptrace{SWS_CORE_01918}
    using const_iterator = const element_type*;
    /// @uptrace{SWS_CORE_01919}
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @uptrace{SWS_CORE_01920}
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    /// @uptrace{SWS_CORE_01931}
    static constexpr size_type extent = Extent;

    /// @uptrace{SWS_CORE_01941}
    template <typename U = void, typename = typename std::enable_if<Extent == dynamic_extent || Extent == 0, U>::type>
    constexpr Span() noexcept : mData(nullptr), mExtent(0)
    {
        xtrace("__ Span()\n");
    }

    /// @uptrace{SWS_CORE_01942}
    constexpr Span(pointer ptr, size_type count) : mData(ptr), mExtent(count)
    {
        xtrace("__ Span(T*, size_t)\n");
        xtrace("____ ptr = ", ptr, ", count = ", count, " (ex = ", Extent, ")\n");
        if (Extent != dynamic_extent && count != Extent) {
            assert(!"inconsistent extent");
        }
    }

    /// @uptrace{SWS_CORE_01943}
    constexpr Span(pointer firstElem, pointer lastElem)
        : Span(firstElem, static_cast<std::size_t>(lastElem - firstElem))
    {
        xtrace("__ Span(T*, T*)\n");
        assert(std::distance(firstElem, lastElem) >= 0);
    }

    /// @uptrace{SWS_CORE_01944}
    template <std::size_t N, typename = typename std::enable_if<
                                 (extent == dynamic_extent || extent == N) &&
                                 std::is_convertible<std::remove_pointer_t<type_of_data<element_type (&)[N]>> (*)[],
                                                     element_type (*)[]>::value>::type>
    constexpr Span(element_type (&arr)[N]) noexcept : Span(std::addressof(arr[0]), N)
    {
        xtrace("__ Span(raw)\n");
    }

    /// @uptrace{SWS_CORE_01953}
    template <
        typename U, std::size_t N,
        typename = typename std::enable_if<
            (extent == dynamic_extent || extent == N) &&
            std::is_convertible<typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                                element_type (*)[]>::value>::type>
    constexpr Span(std::array<U, N>& arr) noexcept : Span(arr.data(), N)
    {
        xtrace("__ Span(std::array&)\n");
    }

    /// @uptrace{SWS_CORE_01954}
    template <
        typename U, std::size_t N,
        typename = typename std::enable_if<
            (extent == dynamic_extent || extent == N) &&
            std::is_convertible<typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                                element_type (*)[]>::value>::type>
    constexpr Span(const std::array<U, N>& arr) noexcept : Span(arr.data(), N)
    {
        xtrace("__ Span(std::array const&)\n");
    }

    /// @uptrace{SWS_CORE_01945}
    template <
        typename U, std::size_t N,
        typename = typename std::enable_if<
            (extent == dynamic_extent || extent == N) &&
            std::is_convertible<typename std::remove_pointer_t<decltype(std::declval<Array<U, N>>().data())> (*)[],
                                element_type (*)[]>::value>::type>
    constexpr Span(Array<U, N>& arr) noexcept : Span(arr.data(), N)
    {
        xtrace("__ Span(Array&)\n");
    }

    /// @uptrace{SWS_CORE_01946}
    template <typename U, std::size_t N,
              typename = typename std::enable_if<
                  (extent == dynamic_extent || extent == N) &&
                  std::is_convertible<
                      typename std::remove_pointer_t<decltype(std::declval<const Array<U, N>>().data())> (*)[],
                      element_type (*)[]>::value>::type>
    constexpr Span(const Array<U, N>& arr) noexcept : Span(arr.data(), N)
    {
        xtrace("__ Span(Array const&)\n");
    }

    /// @uptrace{SWS_CORE_01947}
    template <typename Container, typename = typename std::enable_if<
                                      (extent == dynamic_extent) &&
                                      detail::is_suitable_container<Container, Container&, element_type>::value>::type>
    constexpr Span(Container& cont) : Span(core::data(cont), core::size(cont))
    {
        xtrace("__ Span(Container&)\n");
    }

    /// @uptrace{SWS_CORE_01948}
    template <typename Container,
              typename = typename std::enable_if<
                  (extent == dynamic_extent) &&
                  detail::is_suitable_container<Container, const Container&, element_type>::value>::type>
    constexpr Span(const Container& cont) : Span(core::data(cont), core::size(cont))
    {
        xtrace("__ Span(Container const&)\n");
    }

    /// @uptrace{SWS_CORE_01949}
    constexpr Span(const Span& other) noexcept = default;

    /// @brief Converting constructor
    /// @uptrace{SWS_CORE_01950, 985e58d0fa538e3b40e1474882d63e3683002546}
    template <typename U, std::size_t N,
              typename = typename std::enable_if<(Extent == dynamic_extent || Extent == N) &&
                                                 std::is_convertible<U (*)[], element_type (*)[]>::value>::type*>
    constexpr Span(const Span<U, N>& s) noexcept : Span(s.data(), s.size())
    {
        xtrace("__ converting ctor from N ", N, "\n");
    }

    /// @uptrace{SWS_CORE_01951}
    ~Span() noexcept = default;
    /// @uptrace{SWS_CORE_01952}
    constexpr Span& operator=(const Span& other) noexcept = default;

    /// @uptrace{SWS_CORE_01961}
    template <std::size_t Count>
    constexpr Span<element_type, Count> first() const
    {
        static_assert(Count <= Extent, "Span::first out of bounds");
        assert(Count <= size());
        return {mData, Count};
    }

    /// @uptrace{SWS_CORE_01962}
    constexpr Span<element_type, dynamic_extent> first(size_type count) const
    {
        assert(count <= size());
        return {mData, count};
    }

    /// @uptrace{SWS_CORE_01963}
    template <std::size_t Count>
    constexpr Span<element_type, Count> last() const
    {
        static_assert(Count <= Extent, "Span::last out of bounds");
        assert(Count <= size());
        return {mData + (size() - Count), Count};
    }

    /// @uptrace{SWS_CORE_01964}
    constexpr Span<element_type, dynamic_extent> last(size_type count) const
    {
        assert(count <= size());
        return {mData + (size() - count), count};
    }

    /// @uptrace{SWS_CORE_01965}
    template <std::size_t Offset, std::size_t Count = dynamic_extent>
    constexpr auto subspan() const
        -> Span<element_type,
                (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)>
    {
        static_assert(Offset <= Extent && (Count == dynamic_extent || Count <= Extent - Offset),
                      "Span::subspan out of bounds");
        assert(Offset <= size());
        assert(Count == dynamic_extent || (Count <= size() - Offset));

        constexpr size_type E =
            (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent);

        return Span<element_type, E>(mData + Offset, Count != dynamic_extent ? Count : size() - Offset);
    }

    /// @uptrace{SWS_CORE_01966}
    constexpr Span<element_type, dynamic_extent> subspan(size_type offset, size_type count = dynamic_extent) const
    {
        assert(offset <= size());
        assert(count == dynamic_extent || (count <= size() - offset));

        return {mData + offset, count == dynamic_extent ? size() - offset : count};
    }

    /// @uptrace{SWS_CORE_01967}
    constexpr size_type size() const noexcept
    {
        return mExtent;
    }

    /// @uptrace{SWS_CORE_01968}
    constexpr size_type size_bytes() const noexcept
    {
        return mExtent * sizeof(T);
    }

    /// @uptrace{SWS_CORE_01969}
    constexpr bool empty() const noexcept
    {
        return mExtent == 0;
    }

    /// @uptrace{SWS_CORE_01970}
    constexpr reference operator[](size_type idx) const
    {
        return mData[idx];
    }

    /// @uptrace{SWS_CORE_01959}
    constexpr reference front() const
    {
        return mData[0];
    }

    /// @uptrace{SWS_CORE_01960}
    constexpr reference back() const
    {
        return mData[size() - 1];
    }

    /// @uptrace{SWS_CORE_01971}
    constexpr pointer data() const noexcept
    {
        return mData;
    }

    /// @uptrace{SWS_CORE_01972}
    constexpr iterator begin() const noexcept
    {
        return &mData[0];
    }

    /// @uptrace{SWS_CORE_01973}
    constexpr iterator end() const noexcept
    {
        return &mData[mExtent];
    }

    /// @uptrace{SWS_CORE_01974}
    constexpr const_iterator cbegin() const noexcept
    {
        return &mData[0];
    }

    /// @uptrace{SWS_CORE_01975}
    constexpr const_iterator cend() const noexcept
    {
        return &mData[mExtent];
    }

    /// @uptrace{SWS_CORE_01976}
    constexpr reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01977}
    constexpr reverse_iterator rend() const noexcept
    {
        return reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01978}
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01979}
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

  private:
    pointer mData;
    size_type mExtent;
};

#if defined(__cpp_deduction_guides) && (__cpp_deduction_guides - 0) >= 201703L
template <typename T, std::size_t N>
Span(T (&)[N]) -> Span<T, N>;

template <typename T, std::size_t N>
Span(Array<T, N>&) -> Span<T, N>;

template <typename T, std::size_t N>
Span(const Array<T, N>&) -> Span<const T, N>;

template <typename Container>
Span(Container&) -> Span<typename Container::value_type>;

template <typename Container>
Span(const Container&) -> Span<const typename Container::value_type>;
#endif  // C++17 deduction guides

/// @uptrace{SWS_CORE_01980}
template <typename ElementType, std::size_t Extent>
Span<const Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>  //
as_bytes(Span<ElementType, Extent> s) noexcept
{
    return {reinterpret_cast<const Byte*>(s.data()), s.size_bytes()};
}

/// @uptrace{SWS_CORE_01981}
template <typename ElementType, std::size_t Extent>
Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>  //
as_writable_bytes(Span<ElementType, Extent> s) noexcept
{
    return {reinterpret_cast<Byte*>(s.data()), s.size_bytes()};
}

/// @uptrace{SWS_CORE_01990}
template <typename T>
constexpr Span<T> MakeSpan(T* ptr, typename Span<T>::size_type count)
{
    return Span<T>(ptr, count);
}

/// @uptrace{SWS_CORE_01991}
template <typename T>
constexpr Span<T> MakeSpan(T* firstElem, T* lastElem)
{
    return Span<T>(firstElem, lastElem);
}

/// @uptrace{SWS_CORE_01992}
template <typename T, std::size_t N>
constexpr Span<T, N> MakeSpan(T (&arr)[N]) noexcept
{
    return Span<T, N>(arr);
}

/// @uptrace{SWS_CORE_01993}
template <typename Container>
constexpr Span<typename Container::value_type> MakeSpan(Container& cont)
{
    return Span<typename Container::value_type>(cont);
}

/// @uptrace{SWS_CORE_01994}
template <typename Container>
constexpr Span<typename Container::value_type const> MakeSpan(const Container& cont)
{
    return Span<typename Container::value_type const>(cont);
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_SPAN_H_
