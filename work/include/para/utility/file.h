// ===============================================================================================
// Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
//
// This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
// PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
//
// More information is available at https://www.autosar.io
// ===============================================================================================

#ifndef PARA_UTILITY_FILE_H_
#define PARA_UTILITY_FILE_H_

#include <string>
#include <vector>

#include "para/types/type_alias.h"

namespace para
{
namespace utility
{

enum class perms : uint32_t
{
    /// @brief Deny everything
    none = 0,

    /// @brief owner has read permission
    owner_read = 0400,
    /// @brief owner has write permission
    owner_write = 0200,
    /// @brief owner has execution permission
    owner_exec = 0100,
    /// @brief owner has all permissions
    owner_all = 0700,

    /// @brief group has read permission
    group_read = 040,
    /// @brief group has write permission
    group_write = 020,
    /// @brief group has execution permission
    group_exec = 010,
    /// @brief group has all permissions
    group_all = 070,

    /// @brief others have read permission
    others_read = 04,
    /// @brief others have write permission
    others_write = 02,
    /// @brief others have execution permission
    others_exec = 01,
    /// @brief others have all permissions
    others_all = 07,

    /// @brief all permissions for everyone
    all = 0777,

    /// @brief set uid bit
    /// @note introduction into setgit/setuid: https://en.wikipedia.org/wiki/Setuid
    set_uid = 04000,
    /// @brief set gid bit
    /// @note introduction into setgit/setuid: https://en.wikipedia.org/wiki/Setuid
    set_gid = 02000,
    /// @brief set sticky bit
    /// @note sticky bit introduction: https://en.wikipedia.org/wiki/Sticky_bit
    sticky_bit = 01000,

    /// @brief all permissions for everyone as well as uid, gid and sticky bit
    mask = 07777,

    /// @brief unknown permissions
    unknown = 0xFFFF
};

const std::int32_t kMaxPathLen = 10000;
const std::int32_t kMaxBuffSize = 1024;

/// @brief if path is exist, return true
bool IsExist(const std::string& path);
/// @brief make directory. On success, true is returned.
std::int32_t MakeDir(const std::string& path, const perms& mode);
/// @brief copy file from src to dst
std::int32_t CopyFile(const std::string& src, const std::string& dst, const perms& mode);
/// @brief reculsively copy from src to dst
std::int32_t CopyDirectory(const std::string& src, const std::string& dst, const perms& mode);
/// @deprecated: use CopyDirectory
std::int32_t RecursiveCopy(const std::string src, const std::string dst, const perms& mode);
/// @brief remove file or directory
std::int32_t Remove(const std::string& path);
/// @brief get all file's name in path without directories.
std::vector<std::string> GetAllFilesNames(const std::string& path);
std::vector<std::string> GetAllFileNames(const std::string& path);
/// @brief get all directories name in path without files.
std::vector<std::string> GetAllDirNames(const std::string& path);
/// @brief get file's extension (path=/home/popcornsar/test.cpp => cpp)
std::string GetFileExt(const std::string& path);
/// @brief get file's name (path=/home/popcornsar/test.cpp => test.cpp)
std::string GetFileName(const std::string& path);
/// @brief get file's extension (path=/home/popcornsar/test.cpp => test)
std::string GetFileNameWithoutExt(const std::string& path);
/// @brief get file size
std::int32_t GetFileSize(const std::string& path);
/// @brief set file size
std::int32_t SetFileSize(const std::int32_t size, const std::string& path);
/// @brief create file from vec
void ByteVecToFile(const std::string& file, std::vector<Byte>& vec);
/// @brief create vec from file
void FileToByteVec(const std::string& file, std::vector<Byte>& vec);
std::vector<Byte> FileToByteVec(const std::string& file);
/// @brief get total disk size
std::uint64_t GetTotalDiskSize(std::string path);
/// @brief get available disk size
std::uint64_t GetAvailDiskSize(std::string path);
/// @brief get used disk size
std::uint64_t GetUsedDiskSize(std::string path);
/// @brief list all files matching with pattern
std::vector<std::string> ListMatchingFiles(const std::string& path, const std::string& pattern, bool recursive);

}  // namespace utility
}  // namespace para

#endif  // PARA_UTILITY_FILE_H_
