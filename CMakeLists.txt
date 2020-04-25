cmake_minimum_required(VERSION 3.14)

project(dtl_type LANGUAGES C VERSION 0.1.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(../adt/cmake/BuildTypes.cmake)
    include(../adt/cmake/LeakCheck.cmake)
    include(../adt/cmake/UnitTest.cmake)
endif()

if (LEAK_CHECK)
    message(STATUS "LEAK_CHECK=${LEAK_CHECK} (DTL_TYPE)")
endif()

if (UNIT_TEST)
    message(STATUS "UNIT_TEST=${UNIT_TEST} (DTL_TYPE)")
endif()

### Library dtl_type
set (DTL_TYPE_HEADER_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_av.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_dv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_error.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_hv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_sv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/dtl_type.h
)

set (DTL_TYPE_SOURCE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_av.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_dv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_hv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dtl_sv.c
)

add_library(dtl_type ${DTL_TYPE_SOURCE_LIST} ${DTL_TYPE_HEADER_LIST})

target_include_directories(dtl_type PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if (UNIT_TEST)
    target_compile_definitions(dtl_type PRIVATE UNIT_TEST)
endif()

if (LEAK_CHECK)
    target_compile_definitions(dtl_type PRIVATE MEM_LEAK_CHECK)
    target_link_libraries(dtl_type PRIVATE cutil)
endif()

target_link_libraries(dtl_type PRIVATE adt)
###

### Executable dtl_type_unit
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(../adt ${CMAKE_CURRENT_BINARY_DIR}/adt)
    add_subdirectory(../cutil ${CMAKE_CURRENT_BINARY_DIR}/cutil)

    if (UNIT_TEST)
        set (DTL_TYPE_SUITE_LIST
            test/testsuite_dtl_av.c
            test/testsuite_dtl_dv.c
            test/testsuite_dtl_hv.c
            test/testsuite_dtl_sv.c
        )

        add_executable(dtl_type_unit test/test_main.c ${DTL_TYPE_SUITE_LIST} )

        target_link_libraries(dtl_type_unit PRIVATE dtl_type adt cutest cutil)

        target_include_directories(dtl_type_unit PRIVATE
                                "${PROJECT_BINARY_DIR}"
                                "${CMAKE_CURRENT_SOURCE_DIR}/inc"
                                )
        target_compile_definitions(dtl_type_unit PRIVATE UNIT_TEST)
        if (LEAK_CHECK)
            target_compile_definitions(dtl_type_unit PRIVATE MEM_LEAK_CHECK)
            target_link_libraries(dtl_type_unit PRIVATE cutil)
        endif()

        enable_testing()
        add_test(dtl_type_test ${CMAKE_CURRENT_BINARY_DIR}/dtl_type_unit)
        set_tests_properties(dtl_type_test PROPERTIES PASS_REGULAR_EXPRESSION "OK \\([0-9]+ tests\\)")

    endif()
endif()
