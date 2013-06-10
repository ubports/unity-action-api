cmake_minimum_required(VERSION 2.8.9)

# Default install location. Must be set here, before setting the project.
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "" FORCE)
endif()

project(unity-action CXX)

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

option(basic_warnings "Basic compiler warnings." ON)
include(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -fno-permissive -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "relwithdebinfo")
    option(Werror "Treat warnings as errors" ON)
else()
    option(Werror "Treat warnings as errors" OFF)
endif()

if (Werror)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

if(${basic_warnings})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

# Standard install paths
include(GNUInstallDirs)

set(LIBDIR "lib" CACHE PATH "Destination install dir for the library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# enable QML debugging
string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_lower) # Build types should always be lowercase but sometimes they are not.
if(cmake_build_type_lower MATCHES "debug")
    add_definitions(-DQT_QML_DEBUG)
endif()

# Make sure we have all the needed symbols
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,-z,defs")

# Static C++ checks
add_custom_target(cppcheck COMMAND cppcheck --enable=all -q --error-exitcode=2
                                       ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/tests)
find_package(Qt5Quick REQUIRED)

include_directories(include)
add_subdirectory(src)
add_subdirectory(qml)
add_subdirectory(documentation)

find_program(qmltestrunner_exe qmltestrunner)

if(NOT qmltestrunner_exe)
  msg(FATAL_ERROR "Could not locate qmltestrunner.")
endif()

macro(add_qml_test COMPONENT_NAME)
    set(options NO_ADD_TEST NO_TARGETS)

    cmake_parse_arguments(qmltest "${options}" "IMPORT_PATH" "TARGETS" ${ARGN})

    set(qmltest_TARGET test${COMPONENT_NAME})
    set(qmltest_FILE tst_${COMPONENT_NAME})

    if("${qmltest_IMPORT_PATH}" STREQUAL "")
        add_custom_target(${qmltest_TARGET}
            ${qmltestrunner_exe}  -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
                          -o -,txt)
    else()
        add_custom_target(${qmltest_TARGET}
            ${qmltestrunner_exe} -input ${CMAKE_CURRENT_SOURCE_DIR}/${qmltest_FILE}.qml
                          -import ${qmltest_IMPORT_PATH}
                          -o ${CMAKE_BINARY_DIR}/${qmltest_TARGET}.xml,xunitxml
                          -o -,txt)
    endif()

    if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
        set_target_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
    elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
        set_target_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
    endif()

    if("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")
        add_test(${qmltest_TARGET} make ${qmltest_TARGET})

        if(NOT "${qmltest_UNPARSED_ARGUMENTS}" STREQUAL "")
            set_tests_properties(${qmltest_TARGET} ${qmltest_PROPERTIES})
        elseif(NOT "${qmltest_DEFAULT_PROPERTIES}" STREQUAL "")
            set_tests_properties(${qmltest_TARGET} ${qmltest_DEFAULT_PROPERTIES})
        endif()
    endif("${qmltest_NO_ADD_TEST}" STREQUAL FALSE AND NOT "${qmltest_DEFAULT_NO_ADD_TEST}" STREQUAL "TRUE")

    if("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
        if(NOT "${qmltest_TARGETS}" STREQUAL "")
            foreach(TARGET ${qmltest_TARGETS})
                add_dependencies(${TARGET} ${qmltest_TARGET})
            endforeach(TARGET)
        elseif(NOT "${qmltest_DEFAULT_TARGETS}" STREQUAL "")
            foreach(TARGET ${qmltest_DEFAULT_TARGETS})
                add_dependencies(${TARGET} ${qmltest_TARGET})
            endforeach(TARGET)
        endif()
    endif("${qmltest_NO_TARGETS}" STREQUAL "FALSE")
endmacro(add_qml_test)

enable_testing()
add_subdirectory(test)

include(EnableCoverageReport)
#####################################################################
# Enable code coverage calculation with gcov/gcovr/lcov
# Usage:
#  * Switch build type to coverage (use ccmake or cmake-gui)
#  * Invoke make, make test, make coverage (or ninja if you use that backend)
#  * Find html report in subdir coveragereport
#  * Find xml report feasible for jenkins in coverage.xml
#####################################################################
if(cmake_build_type_lower MATCHES coverage)
  ENABLE_COVERAGE_REPORT(TARGETS unity-action-qt unity-action-qml
                         FILTER /usr/include ${CMAKE_SOURCE_DIR}/test/* ${CMAKE_BINARY_DIR}/*)
endif()
