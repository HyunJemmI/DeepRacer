# ================================================================================================
# Copyright 2024. PopcornSAR Co.,Ltd. All rights reserved.
#
# This software is copyright protected and proprietary to PopcornSAR Co.,Ltd.
# PopcornSAR Co.,Ltd. grants to you only those rights as set out in the license conditions.
#
# More information is available at https://www.autosar.io
# ================================================================================================

#[=======================================================================[.rst:
Para
-------

PARA Application Helper
#]=======================================================================]

#[=======================================================================[.rst:
.. command:: Para_Logo

  Print the PARA logo

  .. code-block:: cmake

    Para_Logo()
#]=======================================================================]
function(Para_Logo)
    if (NOT WIN32)
        string(ASCII 27 ESC)
        set(RED "${ESC}[31m")
        set(GREEN "${ESC}[32m")
        set(BLUE "${ESC}[34m")
        set(WHITE "${ESC}[37m")
        set(RESET "${ESC}[39m")
    endif ()

    message("")
    message("  ${WHITE}██████")
    message("  ${WHITE}██   ██ ${GREEN} █████  ${BLUE}██████  ${RED} █████")
    message("  ${WHITE}██████  ${GREEN}██   ██ ${BLUE}██   ██ ${RED}██   ██")
    message("  ${WHITE}██      ${GREEN}███████ ${BLUE}██████  ${RED}███████")
    message("  ${WHITE}██      ${GREEN}██   ██ ${BLUE}██   ██ ${RED}██   ██")
    message("${RESET}")
endfunction()

#[=======================================================================[.rst:
.. command:: Para_PrintCMakeOptions

  Print the CMake options for PARA adaptive application project

  .. code-block:: cmake

    Para_PrintCMakeOptions()
#]=======================================================================]
function(Para_PrintCMakeOptions)
    message("** [PARA] CMake options:")
    message("   - PARA_SDK ................ : ${PARA_SDK}")
    message("   - CMAKE_BUILD_TYPE ........ : ${CMAKE_BUILD_TYPE}")
    message("   - CMAKE_TOOLCHAIN_FILE .... : ${CMAKE_TOOLCHAIN_FILE}")
    message("   - CMAKE_INSTALL_PREFIX .... : ${CMAKE_INSTALL_PREFIX}")
    message("   - CMake version ........... : ${CMAKE_VERSION}")
endfunction()

#[=======================================================================[.rst:
.. command:: Para_PrintBuildOptions

  Print the build options for PARA adaptive application project

  .. code-block:: cmake

    Para_PrintBuildOptions()
#]=======================================================================]
function(Para_PrintBuildOptions)
    message("** [PARA] Build properties:")
    message("   - Platform ................ : ${CMAKE_SYSTEM_NAME}")
    message("   - C++ compiler ............ : ${CMAKE_CXX_COMPILER}")
    message("   - C++ compiler version .... : ${CMAKE_CXX_COMPILER_VERSION}")
    message("   - C++ standard ............ : ${CMAKE_CXX_STANDARD}")
    message("   - C++ standard library .... : ${CMAKE_CXX_STANDARD_LIBRARIES}")
    message("   - C++ flags ............... : ${CMAKE_CXX_FLAGS}")
endfunction()

#[=======================================================================[.rst:
.. command:: Para_Init

  Initialize PARA

  .. code-block:: cmake

    Para_Init()
#]=======================================================================]
macro(Para_Init)
    if (NOT DEFINED PARA)
        set(PARA para)

        if (NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE Release)
        endif ()

        list(APPEND CMAKE_PREFIX_PATH ${PARA_SDK})

        if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
            set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default installation path" FORCE)
        endif ()

        # C++ Standard: ISO C++14
        set(CMAKE_CXX_STANDARD 14)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        Para_Logo()
        Para_PrintCMakeOptions()
        Para_PrintBuildOptions()
    else ()
        message(WARNING "PARA already initialized")
    endif ()
endmacro()

#[=======================================================================[.rst:
.. command:: Para_App

  Configure a PARA adaptive application

  .. code-block:: cmake

    Para_App()
#]=======================================================================]
macro(Para_App)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(PARAMS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT DEFINED PARA_APP_NAME)
        message(FATAL_ERROR "[PARA] ERROR: The variable 'PARA_APP_NAME' is not set")
    endif ()

    if (NOT DEFINED PARA_APP_VERSION)
        message(FATAL_ERROR "[PARA] ERROR: The variable 'PARA_APP_VERSION' is not set")
    endif ()

    if (NOT DEFINED PARA_APP_GEN_DIR)
        message(FATAL_ERROR "[PARA] ERROR: The variable 'PARA_APP_GEN_DIR' is not set")
    endif ()

    message("** [PARA] Configuring the PARA adaptive application: ${PARA_APP_NAME}@${PARA_APP_VERSION}")
    message("   - Generated artifacts:\n      ${PARA_APP_GEN_DIR}")

    set(PARA_PLATFORM_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(PARA_PLATFORM_INSTALL_BINDIR ${PARA_PLATFORM_INSTALL_PREFIX}/bin)
    set(PARA_PLATFORM_INSTALL_LIBDIR ${PARA_PLATFORM_INSTALL_PREFIX}/lib)
    set(PARA_PLATFORM_INSTALL_SYSCONFDIR ${PARA_PLATFORM_INSTALL_PREFIX}/etc)
    set(PARA_PLATFORM_INSTALL_LOCALSTATEDIR ${PARA_PLATFORM_INSTALL_PREFIX}/var)

    set(PARA_APP_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/opt/${PARA_APP_NAME})
    set(PARA_APP_INSTALL_BINDIR ${PARA_APP_INSTALL_PREFIX}/bin)
    set(PARA_APP_INSTALL_LIBDIR ${PARA_APP_INSTALL_PREFIX}/lib)
    set(PARA_APP_INSTALL_SYSCONFDIR ${PARA_APP_INSTALL_PREFIX}/etc)
    set(PARA_APP_INSTALL_LOCALSTATEDIR ${PARA_APP_INSTALL_PREFIX}/var)

    # Install generated manifests
    if (DEFINED PARA_PLATFORM)
        install(
                DIRECTORY ${PARA_APP_GEN_DIR}/manifest/
                DESTINATION ${PARA_PLATFORM_INSTALL_SYSCONFDIR}
        )
    else ()
        install(
                DIRECTORY ${PARA_APP_GEN_DIR}/manifest/
                DESTINATION ${PARA_APP_INSTALL_SYSCONFDIR}
        )
    endif ()
endmacro()

#[=======================================================================[.rst:
.. command:: Para_Target

  Configure a binary target declared as add_executable() or add_library()

  .. code-block:: cmake

    Para_Target(<target>
    [[PARA_GEN_DIR <dir>]
     [OUTPUT_NAME <name>]
     [PARA_LIBS_PUBLIC [items1...]]
     [PARA_LIBS_PRIVATE [items1...]]
    ]
    )
#]=======================================================================]
function(Para_Target PARAMS_TARGET)
    set(options)
    set(oneValueArgs PARA_GEN_DIR OUTPUT_NAME)
    set(multiValueArgs PARA_LIBS_PUBLIC PARA_LIBS_PRIVATE)
    cmake_parse_arguments(PARAMS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message("** [PARA] Configuring the target: ${PARAMS_TARGET}")

    get_target_property(TARGET_COMPILE_OPTIONS ${PARAMS_TARGET} COMPILE_OPTIONS)
    if (NOT TARGET_COMPILE_OPTIONS STREQUAL TARGET_COMPILE_OPTIONS-NOTFOUND)
        message("   - Compiler options ........ : ${TARGET_COMPILE_OPTIONS}")
    endif ()

    if (DEFINED PARAMS_PARA_GEN_DIR)
        # Include generated headers
        target_include_directories(${PARAMS_TARGET}
                PRIVATE
                ${PARAMS_PARA_GEN_DIR}/include
        )

        # Install binaries
        if (PARA_PLATFORM STREQUAL TRUE)
            install(TARGETS ${PARAMS_TARGET}
                    RUNTIME DESTINATION ${PARA_PLATFORM_INSTALL_BINDIR}
                    LIBRARY DESTINATION ${PARA_PLATFORM_INSTALL_LIBDIR}
            )
        else ()
            install(TARGETS ${PARAMS_TARGET}
                    RUNTIME DESTINATION ${PARA_APP_INSTALL_BINDIR}
                    LIBRARY DESTINATION ${PARA_APP_INSTALL_LIBDIR}
            )
        endif ()
    endif ()

    # Link dependencies as public
    if (DEFINED PARAMS_PARA_LIBS_PUBLIC)
        message("   - Public dependencies:")
        foreach (lib ${PARAMS_PARA_LIBS_PUBLIC})
            message("      ${PARA}-${lib}")
            find_package(${PARA}-${lib} REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)
            target_link_libraries(${PARAMS_TARGET}
                    PUBLIC
                    ${PARA}::${lib}
            )
        endforeach ()
    endif ()

    # Link dependencies as private
    if (DEFINED PARAMS_PARA_LIBS_PRIVATE)
        message("   - Private dependencies:")
        foreach (lib ${PARAMS_PARA_LIBS_PRIVATE})
            message("      ${PARA}-${lib}")
            find_package(${PARA}-${lib} REQUIRED CMAKE_FIND_ROOT_PATH_BOTH)
            target_link_libraries(${PARAMS_TARGET}
                    PRIVATE
                    ${PARA}::${lib}
            )
        endforeach ()
    endif ()

    # Install the application
    if (DEFINED PARAMS_OUTPUT_NAME)
        set_target_properties(${PARAMS_TARGET}
                PROPERTIES
                OUTPUT_NAME ${PARAMS_OUTPUT_NAME}
        )
    endif ()
endfunction()

Para_Init()