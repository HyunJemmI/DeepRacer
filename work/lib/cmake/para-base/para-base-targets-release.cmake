#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "para::base" for configuration "Release"
set_property(TARGET para::base APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(para::base PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libpara_base.so.1.0.0"
  IMPORTED_SONAME_RELEASE "libpara_base.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS para::base )
list(APPEND _IMPORT_CHECK_FILES_FOR_para::base "${_IMPORT_PREFIX}/lib/libpara_base.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
