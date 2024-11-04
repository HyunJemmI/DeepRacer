// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_TYPES_TYPE_ALIAS_H_
#define PARA_TYPES_TYPE_ALIAS_H_

#include <cstdint>
#include <vector>

namespace para
{

using Byte = std::uint8_t;
using Payload = std::vector<std::uint8_t>;

}  // namespace para

#endif  // PARA_TYPES_TYPE_ALIAS_H_