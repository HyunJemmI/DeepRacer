// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_UTILITY_H_
#define ARA_CORE_UTILITY_H_

#include <climits>
#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace ara
{
namespace core
{

namespace detail
{

#ifdef __GNUC__
#ifndef ATTR_PACKED
#define PARA_PACKED __attribute__((packed))
#else
// Do here what is necessary for achieving SWS_CORE_10101 for this compiler!
#define PARA_PACKED
#endif
#endif

/// @brief A non-integral binary type
/// @uptrace{SWS_CORE_10100}
/// @uptrace{SWS_CORE_10101}
/// @uptrace{SWS_CORE_10105}
class Byte final
{
    /// @uptrace{SWS_CORE_10102}
    unsigned char mValue;

  public:
    /// @brief Default constructor
    /// @uptrace{SWS_CORE_10104}
    Byte() noexcept = default;

    /// @brief Converting constructor from 'unsigned char'
    /// @uptrace{SWS_CORE_10103}
    /// @uptrace{SWS_CORE_10106}
    constexpr explicit Byte(unsigned char value) noexcept : mValue(value)
    {}

    /// @brief Conversion operator to 'unsigned char'
    /// @uptrace{SWS_CORE_10107}
    /// @uptrace{SWS_CORE_10108}
    constexpr explicit operator unsigned char() const noexcept
    {
        return mValue;
    }
} PARA_PACKED;

/// @uptrace{SWS_CORE_10109}
constexpr bool operator==(Byte b1, Byte b2) noexcept
{
    return (static_cast<unsigned char>(b1) == static_cast<unsigned char>(b2));
}

/// @uptrace{SWS_CORE_10110}
constexpr bool operator!=(Byte b1, Byte b2) noexcept
{
    return (static_cast<unsigned char>(b1) != static_cast<unsigned char>(b2));
}

}  // namespace detail

#if __cplusplus < 201703L
/// @uptrace{SWS_CORE_04200}
using Byte = detail::Byte;
#else
/// @uptrace{SWS_CORE_04200}
using Byte = std::byte;
#endif

/// @uptrace{SWS_CORE_04011}
struct in_place_t
{
    /// @uptrace{SWS_CORE_04012}
    explicit in_place_t() = default;
};

/// @uptrace{SWS_CORE_04013}
constexpr in_place_t in_place{};

/// @uptrace{SWS_CORE_04021}
template <typename T>
struct in_place_type_t
{
    /// @uptrace{SWS_CORE_04022}
    explicit in_place_type_t() = default;
};

/// @uptrace{SWS_CORE_04023}
template <typename T>
constexpr in_place_type_t<T> in_place_type{};

/// @uptrace{SWS_CORE_04031}
template <std::size_t I>
struct in_place_index_t
{
    /// @uptrace{SWS_CORE_04032}
    explicit in_place_index_t() = default;
};

/// @uptrace{SWS_CORE_04033}
template <std::size_t I>
constexpr in_place_index_t<I> in_place_index{};

#if defined(__cpp_lib_nonmember_container_access) && (__cpp_lib_nonmember_container_access - 0) >= 201411
using std::data;
using std::empty;
using std::size;
#else
// pre C++17

/// @brief Return a pointer to the block of memory that contains the elements of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns a pointer to the first element of the container
/// @uptrace{SWS_CORE_04110}
template <typename Container>
constexpr auto data(Container& c) -> decltype(c.data())
{
    return c.data();
}

/// @brief Return a const_pointer to the block of memory that contains the elements of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns a pointer to the first element of the container
/// @uptrace{SWS_CORE_04111}
template <typename Container>
constexpr auto data(const Container& c) -> decltype(c.data())
{
    return c.data();
}

/// @brief Return a pointer to the block of memory that contains the elements of a raw array.
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  reference to a raw array
/// @returns a pointer to the first element of the array
/// @uptrace{SWS_CORE_04112}
template <typename T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept
{
    return array;
}

/// @brief Return a pointer to the block of memory that contains the elements of a std::initializer_list.
/// @tparam E  the type of elements in the std::initializer_list
/// @param il  the std::initializer_list
/// @returns a pointer to the first element of the std::initializer_list
/// @uptrace{SWS_CORE_04113}
template <typename E>
constexpr const E* data(std::initializer_list<E> il) noexcept
{
    return il.begin();
}

/// @brief Return the size of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns the size of the container
/// @uptrace{SWS_CORE_04120}
template <typename Container>
constexpr auto size(const Container& c) -> decltype(c.size())
{
    return c.size();
}

/// @brief Return the size of a raw array.
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  reference to a raw array
/// @returns the size of the array, i.e. N
/// @uptrace{SWS_CORE_04121}
template <typename T, std::size_t N>
constexpr std::size_t size(T const (&array)[N]) noexcept
{
    static_cast<void>(array);
    return N;
}

/// @brief Return whether the given container is empty.
/// @tparam Container  a type with a empty() method
/// @param c  an instance of Container
/// @returns true if the container is empty, false otherwise
/// @uptrace{SWS_CORE_04130}
template <typename Container>
constexpr auto empty(const Container& c) -> decltype(c.empty())
{
    return c.empty();
}

/// @brief Return whether the given raw array is empty.
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  the raw array
/// @returns false
/// @uptrace{SWS_CORE_04131}
template <typename T, std::size_t N>
constexpr bool empty(T const (&array)[N]) noexcept
{
    static_cast<void>(array);
    return false;
}

/// @brief Return whether the given std::initializer_list is empty.
/// @tparam E  the type of elements in the std::initializer_list
/// @param il  the std::initializer_list
/// @returns true if the std::initializer_list is empty, false otherwise
/// @uptrace{SWS_CORE_04132}
template <typename E>
constexpr bool empty(std::initializer_list<E> il) noexcept
{
    return il.size() == 0;
}
#endif

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_UTILITY_H_
