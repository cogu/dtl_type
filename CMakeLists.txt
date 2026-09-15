cmake_minimum_required(VERSION 3.20)

project(dtl_type LANGUAGES C VERSION 0.3.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    option(ENABLE_MSVC_ANALYZE "Enable MSVC static code analysis (/analyze)" OFF)
    if(ENABLE_MSVC_ANALYZE)
        add_compile_options(/analyze)
    endif()
else()
    add_compile_options(-Wall -Wextra)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
    endif()

    option(UNIT_TEST "Unit Test Build" OFF)
    option(LEAK_CHECK "Memory Leak Detection" OFF)
endif()

set(DTL_TYPE_SANITIZERS "" CACHE STRING "Comma-separated sanitizers to enable (for example: address,undefined or thread)")
set_property(CACHE DTL_TYPE_SANITIZERS PROPERTY STRINGS "" "address" "address,undefined" "thread")

if(DTL_TYPE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(-fsanitize=${DTL_TYPE_SANITIZERS} -fno-omit-frame-pointer)
        add_link_options(-fsanitize=${DTL_TYPE_SANITIZERS})
    else()
        message(FATAL_ERROR "DTL_TYPE_SANITIZERS is only supported with GCC or Clang")
    endif()
endif()

if (UNIT_TEST)
    message(STATUS "UNIT_TEST=${UNIT_TEST} (DTL_TYPE)")
endif()

if (LEAK_CHECK)
    message(STATUS "LEAK_CHECK=${LEAK_CHECK} (DTL_TYPE)")
endif()

### Library dtl_type
set (DTL_TYPE_HEADER_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_av.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_dv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_error.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_hv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_sv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dtl_type.h
)

set (DTL_TYPE_SOURCE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_av.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_dv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_hv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_sv.c
)

add_library(dtl_type ${DTL_TYPE_HEADER_LIST} ${DTL_TYPE_SOURCE_LIST})

if (UNIT_TEST)
    target_compile_definitions(dtl_type PRIVATE UNIT_TEST)
endif()

if (LEAK_CHECK)
    target_compile_definitions(dtl_type PRIVATE MEM_LEAK_CHECK)
    target_link_libraries(dtl_type PRIVATE cutil)
endif()

target_link_libraries(dtl_type PUBLIC adt)
target_include_directories(dtl_type PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
###

### Executable dtl_type_unit
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(NOT TARGET adt)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../adt/CMakeLists.txt")
            add_subdirectory(../adt ${CMAKE_CURRENT_BINARY_DIR}/adt)
        else()
            message(FATAL_ERROR "adt repository not found at ../adt")
        endif()
    endif()

    if(NOT TARGET cutil)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../cutil/CMakeLists.txt")
            add_subdirectory(../cutil ${CMAKE_CURRENT_BINARY_DIR}/cutil)
        else()
            message(FATAL_ERROR "cutil repository not found at ../cutil")
        endif()
    endif()

    if(UNIT_TEST)
        set (DTL_TYPE_TEST_SUITE_LIST
            test/testsuite_dtl_av.c
            test/testsuite_dtl_dv.c
            test/testsuite_dtl_hv.c
            test/testsuite_dtl_sv.c
        )

        add_executable(dtl_type_unit test/test_main.c ${DTL_TYPE_TEST_SUITE_LIST})
        target_link_libraries(dtl_type_unit PRIVATE adt cutil dtl_type cutest)
        target_include_directories(dtl_type_unit PRIVATE
                                "${PROJECT_BINARY_DIR}"
                                "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                "${CMAKE_CURRENT_SOURCE_DIR}/test"
                                )
        target_compile_definitions(dtl_type_unit PRIVATE UNIT_TEST)

        if (LEAK_CHECK)
            target_compile_definitions(dtl_type_unit PRIVATE MEM_LEAK_CHECK)
            target_link_libraries(dtl_type_unit PRIVATE cutil)
        endif()

        enable_testing()
        add_test(NAME dtl_type_unit COMMAND dtl_type_unit)
        set_tests_properties(dtl_type_unit PROPERTIES
            PASS_REGULAR_EXPRESSION "OK \\([0-9]+ tests\\)"
            LABELS "unit"
        )
    endif()
endif()
###
