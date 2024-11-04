// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_UTILITY_ENDIAN_H_
#define PARA_UTILITY_ENDIAN_H_

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#include "ara/core/array.h"

// Little-endian and big-endian to host-endian conversion utilities.
namespace para
{
namespace utility
{

// Base type for endian conversions to host endianness.
template <typename T, typename Enable = void>
struct HostEndian;

// Specialization for integral types.
template <typename T>
struct HostEndian<T, std::enable_if_t<std::is_integral<T>::value>>
{
    // Returns the given big-endian value in host endianness.
    static constexpr T FromBig(T value)
    {
        const std::size_t N = sizeof(T);

        union
        {
            T value;
            std::uint8_t data[N];
        } u{value};

        return FromBig(u.data, std::make_index_sequence<N>{});
    }

    // Returns the given host-endian value in big endianness.
    static constexpr T ToBig(T value)
    {
        return FromBig(value);
    }

    // Returns the given little-endian value in host endianness.
    static constexpr T FromLittle(T value)
    {
        const std::size_t N = sizeof(T);

        union
        {
            T value;
            std::uint8_t data[N];
        } u{value};

        return FromLittle(u.data, std::make_index_sequence<N>{});
    }

    // Returns the given host-endian value in little endianness.
    static constexpr T ToLittle(T value)
    {
        return FromLittle(value);
    }

  private:
    // Allow other specializations to access private members.
    template <typename, typename>
    friend struct HostEndian;

    // Utilities to shift-or the little-endian or big-endian bytes into a host-endian value.
    template <std::size_t N, std::size_t... Is>
    static constexpr T FromLittle(const std::uint8_t (&value)[N], std::index_sequence<Is...>)
    {
        T out = 0;
        (void)std::initializer_list<bool>{(out |= static_cast<T>(value[Is]) << Is * 8, false)...};
        return out;
    }

    template <std::size_t N, std::size_t... Is>
    static constexpr T FromBig(const std::uint8_t (&value)[N], std::index_sequence<Is...>)
    {
        T out = 0;
        (void)std::initializer_list<bool>{(out |= static_cast<T>(value[Is]) << (N - Is - 1) * 8, false)...};
        return out;
    }
};

// Specialization for floating point types.
template <typename T>
struct HostEndian<T, std::enable_if_t<std::is_floating_point<T>::value>>
{
    static constexpr T FromBig(T value)
    {
        const std::size_t N = sizeof(T);

        union
        {
            T value;
            std::uint8_t data[N];
        } input{value};

        union
        {
            Integral value;
            T native;
        } output{HostEndian<Integral>::FromLittle(input.data, std::make_index_sequence<N>{})};

        return output.native;
    }

    // Returns the given host-endian value in big endianness.
    static constexpr T ToBig(T value)
    {
        return FromBig(value);
    }

    static constexpr T FromLittle(T value)
    {
        const std::size_t N = sizeof(T);

        union
        {
            T value;
            std::uint8_t data[N];
        } input{value};

        union
        {
            Integral value;
            T native;
        } output{HostEndian<Integral>::FromBig(input.data, std::make_index_sequence<N>{})};

        return output.native;
    }

    // Returns the given host-endian value in little endianness.
    static constexpr T ToLittle(T value)
    {
        return FromLittle(value);
    }

  private:
    // Type matching utility to map from floating point to integral type of the same size.
    static std::uint32_t IntegralType(float);
    static std::uint64_t IntegralType(double);

    // Integral type that matches the size of floating point type T.
    using Integral = decltype(IntegralType(std::declval<T>()));
};

// Specialization for vector arithmetic types.
template <typename T, typename Allocator>
struct HostEndian<std::vector<T, Allocator>, std::enable_if_t<std::is_arithmetic<T>::value>>
{
    using Type = std::vector<T, Allocator>;

    // Returns the given big-endian value in host endianness.
    static constexpr void FromBig(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromBig(element);
        }
    }

    // Returns the given host-endian value in big endianness.
    static constexpr void ToBig(Type& value)
    {
        FromBig(value);
    }

    // Returns the given little-endian value in host endianness.
    static constexpr void FromLittle(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromLittle(element);
        }
    }

    // Returns the given host-endian value in little endianness.
    static constexpr void ToLittle(Type& value)
    {
        FromLittle(value);
    }
};

// Specialization for std::array arithmetic types.
template <typename T, std::size_t Length>
struct HostEndian<std::array<T, Length>, std::enable_if_t<std::is_arithmetic<T>::value>>
{
    using Type = std::array<T, Length>;

    // Returns the given big-endian value in host endianness.
    static constexpr void FromBig(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromBig(element);
        }
    }

    // Returns the given host-endian value in big endianness.
    static constexpr void ToBig(Type& value)
    {
        FromBig(value);
    }

    // Returns the given little-endian value in host endianness.
    static constexpr void FromLittle(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromLittle(element);
        }
    }

    // Returns the given host-endian value in little endianness.
    static constexpr void ToLittle(Type& value)
    {
        FromLittle(value);
    }
};

// Specialization for standard array arithmetic types.
template <typename T, std::size_t Length>
struct HostEndian<T[Length], std::enable_if_t<std::is_arithmetic<T>::value>>
{
    using Type = T[Length];

    // Returns the given big-endian value in host endianness.
    static constexpr void FromBig(Type& value)
    {
        for (std::size_t i = 0; i < Length; ++i) {
            HostEndian<T>::FromBig(value[i]);
        }
    }

    // Returns the given host-endian value in big endianness.
    static constexpr void ToBig(Type& value)
    {
        FromBig(value);
    }

    // Returns the given little-endian value in host endianness.
    static constexpr void FromLittle(Type& value)
    {
        for (std::size_t i = 0; i < Length; ++i) {
            HostEndian<T>::FromLittle(value[i]);
        }
    }

    // Returns the given host-endian value in little endianness.
    static constexpr void ToLittle(Type& value)
    {
        FromLittle(value);
    }
};

// Specialization for ara::core::Array arithmetic types.
// Specialization for std::array arithmetic types.
template <typename T, std::size_t Length>
struct HostEndian<ara::core::Array<T, Length>, std::enable_if_t<std::is_arithmetic<T>::value>>
{
    using Type = ara::core::Array<T, Length>;

    // Returns the given big-endian value in host endianness.
    static constexpr void FromBig(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromBig(element);
        }
    }

    // Returns the given host-endian value in big endianness.
    static constexpr void ToBig(Type& value)
    {
        FromBig(value);
    }

    // Returns the given little-endian value in host endianness.
    static constexpr void FromLittle(Type& value)
    {
        for (T& element : value) {
            element = HostEndian<T>::FromLittle(element);
        }
    }

    // Returns the given host-endian value in little endianness.
    static constexpr void ToLittle(Type& value)
    {
        FromLittle(value);
    }
};

}  // namespace utility
}  // namespace para

#endif  // PARA_UTILITY_ENDIAN_H_