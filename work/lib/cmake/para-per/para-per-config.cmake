# ================================================================================================
# Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
#
# This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
# PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
#
# More information is available at https://www.autosar.io
# ================================================================================================

####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was para-per-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

find_dependency(para-base CMAKE_FIND_ROOT_PATH_BOTH)
find_dependency(para-core-types CMAKE_FIND_ROOT_PATH_BOTH)
find_dependency(para-log CMAKE_FIND_ROOT_PATH_BOTH)

include(${CMAKE_CURRENT_LIST_DIR}/para-per-targets.cmake)
