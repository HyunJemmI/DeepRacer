// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_UTILITY_BYTEORDER_H_
#define PARA_UTILITY_BYTEORDER_H_

#include <cstdint>

#if defined(LINUX)
#include <endian.h>
#elif defined(FREEBSD)
#include <sys/endian.h>
#else
// TEST IF THERE COULD BE AN ERROR!
// #error "Undefined OS (only Linux/FreeBSD are currently supported)"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

#define PARA_BYTES_TO_WORD(x0, x1) (std::uint16_t((x0) << 8 | (x1)))
#define PARA_BYTES_TO_LONG(x0, x1, x2, x3) (std::uint32_t((x0) << 24 | (x1) << 16 | (x2) << 8 | (x3)))

#define PARA_WORDS_TO_LONG(x0, x1) (std::uint32_t((x0) << 16 | (x1)))

#define PARA_WORD_BYTE0(x) (std::uint8_t((x) & 0xFF))
#define PARA_WORD_BYTE1(x) (std::uint8_t((x) >> 8))

#define PARA_LONG_BYTE0(x) (std::uint8_t((x) & 0xFF))
#define PARA_LONG_BYTE1(x) (std::uint8_t(((x) >> 8) & 0xFF))
#define PARA_LONG_BYTE2(x) (std::uint8_t(((x) >> 16) & 0xFF))
#define PARA_LONG_BYTE3(x) (std::uint8_t(((x) >> 24) & 0xFF))

#define PARA_LONG_WORD0(x) (std::uint16_t((x) & 0xFFFF))
#define PARA_LONG_WORD1(x) (std::uint16_t(((x) >> 16) & 0xFFFF))

#elif __BYTE_ORDER == __BIG_ENDIAN

#define PARA_BYTES_TO_WORD(x0, x1) (std::uint16_t((x1) << 8 | (x0)))
#define PARA_BYTES_TO_LONG(x0, x1, x2, x3) (std::uint32_t((x3) << 24 | (x2) << 16 | (x1) << 8 | (x0)))

#define PARA_WORD_BYTE0(x) (std::uint8_t((x) >> 8))
#define PARA_WORD_BYTE1(x) (std::uint8_t((x) & 0xFF))

#define PARA_LONG_BYTE0(x) (std::uint8_t(((x) >> 24) & 0xFF))
#define PARA_LONG_BYTE1(x) (std::uint8_t(((x) >> 16) & 0xFF))
#define PARA_LONG_BYTE2(x) (std::uint8_t(((x) >> 8) & 0xFF))
#define PARA_LONG_BYTE3(x) (std::uint8_t((x) & 0xFF))

#define PARA_LONG_WORD0(x) (std::uint16_t((((x) >> 16) & 0xFFFF))
#define PARA_LONG_WORD1(x) (std::uint16_t(((x) & 0xFFFF))

#else

#error "__BYTE_ORDER is not defined!"

#endif

#endif  // PARA_UTILITY_BYTEORDER_H_