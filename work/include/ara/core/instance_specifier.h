// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef ARA_CORE_INSTANCE_SPECIFIER_H
#define ARA_CORE_INSTANCE_SPECIFIER_H

#include <algorithm>

#include "ara/core/core_error_domain.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"

namespace ara
{
namespace core
{
/// @brief class representing an AUTOSAR Instance Specifier, which is basically an AUTOSAR shortname-path wrapper.
/// @uptrace{SWS_CORE_08001}
class InstanceSpecifier final
{
  private:
    ara::core::String mMetaModelIdentifier;

    InstanceSpecifier(StringView metaModelIdentifier, bool /*tag*/) : mMetaModelIdentifier(metaModelIdentifier)
    {}

    /// @uptrace{SWS_CORE_10200}
    /// @uptrace{SWS_CORE_10201}
    static Result<void> Validate(StringView metaModelIdentifier) noexcept
    {
        std::size_t endOffset = static_cast<std::size_t>(-1);  // point this to a "virtual leading '/'"
        do {
            const size_t currentOffset = endOffset + 1;  // skip the '/'
            endOffset = metaModelIdentifier.find('/', currentOffset);
            const size_t len = (endOffset == StringView::npos) ? StringView::npos : endOffset - currentOffset;
            const StringView pathComponent = metaModelIdentifier.substr(currentOffset, len);
            if (pathComponent.empty()) {
                return Result<void>::FromError(CoreErrc::kInvalidMetaModelPath);
            }
            if (!IsValidMetamodelIdentifierPart(pathComponent)) {
                return Result<void>::FromError(CoreErrc::kInvalidMetaModelShortname);
            }
        } while (endOffset != StringView::npos);

        return Result<void>();
    }

    constexpr static bool IsValidStartChar(char ch) noexcept
    {
        return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
    }

    constexpr static bool IsValidChar(char ch) noexcept
    {
        return IsValidStartChar(ch) || (ch >= '0' && ch <= '9') || (ch == '_');
    }

    constexpr static bool IsValidMetamodelIdentifierPart(StringView text) noexcept
    {
        if (text.empty()) {
            return false;
        }
        return IsValidStartChar(text[0]) && std::all_of(text.begin() + 1, text.end(), IsValidChar);
    }

  public:
    /// @uptrace{SWS_CORE_08032}
    /// @uptrace{SWS_CORE_10202}
    static Result<InstanceSpecifier> Create(StringView metaModelIdentifier)
    {
        Result<void> ret = Validate(metaModelIdentifier);
        if (!ret) {
            return Result<InstanceSpecifier>::FromError(ret.Error());
        }
        return InstanceSpecifier(metaModelIdentifier, true);
    }

    /// @brief throwing ctor from meta-model string
    /// @param metaModelIdentifier  stringified meta model identifier (short name path) where path separator is '/'.
    ///        Lifetime of underlying string has to exceed the lifetime of the constructed InstanceSpecifier.
    /// @throws InstanceSpecifierException in case the given metamodel_identifier isn't a valid meta-model
    /// identifier/short name path.
    /// @uptrace{SWS_CORE_08021}
    /// @uptrace{SWS_CORE_10202}
    explicit InstanceSpecifier(StringView metaModelIdentifier)
        : InstanceSpecifier(Create(metaModelIdentifier).ValueOrThrow())
    {}

    /// @uptrace{SWS_CORE_08022}
    InstanceSpecifier(const InstanceSpecifier& other) = default;
    /// @uptrace{SWS_CORE_08023}
    InstanceSpecifier(InstanceSpecifier&& other) noexcept = default;
    /// @uptrace{SWS_CORE_08024}
    InstanceSpecifier& operator=(const InstanceSpecifier& other) = default;
    /// @uptrace{SWS_CORE_08025}
    InstanceSpecifier& operator=(InstanceSpecifier&& other) = default;
    /// @brief Destructor
    /// @uptrace{SWS_CORE_08029}
    ~InstanceSpecifier() = default;

    /// @brief method to return the stringified form of InstanceSpecifier
    /// @return stringified form of InstanceSpecifier. Lifetime of the underlying string is only guaranteed for the
    ///         lifetime of the underlying string of the StringView passed to the constructor.
    /// @uptrace{SWS_CORE_08041}
    StringView ToString() const noexcept
    {
        return mMetaModelIdentifier;
    }

    /// @brief eq operator to compare with other InstanceSpecifier instance.
    /// @param other InstanceSpecifier instance to compare this one with.
    /// @return true in case both InstanceSpecifiers are denoting exactly the same model element, false else.
    /// @uptrace{SWS_CORE_08042}
    bool operator==(const InstanceSpecifier& other) const noexcept
    {
        return mMetaModelIdentifier == other.ToString();
    }

    /// @brief eq operator to compare with other InstanceSpecifier instance.
    /// @param other string representation to compare this one with.
    /// @return true in case this InstanceSpecifiers is denoting exactly the same model element as other, false else.
    /// @uptrace{SWS_CORE_08043}
    bool operator==(StringView other) const noexcept
    {
        return mMetaModelIdentifier == other;
    }

    /// @brief uneq operator to compare with other InstanceSpecifier instance.
    /// @param other InstanceSpecifier instance to compare this one with.
    /// @return false in case both InstanceSpecifiers are denoting exactly the same model element, true else.
    /// @uptrace{SWS_CORE_08044}
    bool operator!=(const InstanceSpecifier& other) const noexcept
    {
        return mMetaModelIdentifier != other.ToString();
    }

    /// @brief uneq operator to compare with other InstanceSpecifier string representation.
    /// @param other string representation to compare this one with.
    /// @return false in case this InstanceSpecifiers is denoting exactly the same model element as other, true else.
    /// @uptrace{SWS_CORE_08045}
    bool operator!=(StringView other) const noexcept
    {
        return mMetaModelIdentifier != other;
    }

    /// @brief lower than operator to compare with other InstanceSpecifier for ordering purposes (f.i. when collecting
    /// identifiers in maps).
    /// @param other InstanceSpecifier instance to compare this one with.
    /// @return true in case this InstanceSpecifiers is lexically lower than other, false else.
    /// @uptrace{SWS_CORE_08046}
    bool operator<(const InstanceSpecifier& other) const noexcept
    {
        return mMetaModelIdentifier < other.mMetaModelIdentifier;
    }
};

/// @brief non-member function op to allow char* on lhs.
/// @param lhs  stringified form of a InstanceSpecifier
/// @param rhs  an InstanceSpecifier
/// @returns true in case rhs string representation equals lhs
/// @uptrace{SWS_CORE_08081}
inline bool operator==(StringView lhs, const InstanceSpecifier& rhs) noexcept
{
    return lhs == rhs.ToString();
}

/// @brief non-member function op to allow char* on lhs.
/// @param lhs  stringified form of a InstanceSpecifier
/// @param rhs  an InstanceSpecifier
/// @returns true in case rhs string representation not equals lhs
/// @uptrace{SWS_CORE_08082}
inline bool operator!=(StringView lhs, const InstanceSpecifier& rhs) noexcept
{
    return lhs != rhs.ToString();
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_INSTANCE_SPECIFIER_H
