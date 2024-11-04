#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "para::idsm" for configuration "Release"
set_property(TARGET para::idsm APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(para::idsm PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libpara_idsm.so.1.0.0"
  IMPORTED_SONAME_RELEASE "libpara_idsm.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS para::idsm )
list(APPEND _IMPORT_CHECK_FILES_FOR_para::idsm "${_IMPORT_PREFIX}/lib/libpara_idsm.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
