cmake_minimum_required(VERSION 2.8.11)
project(mapper)

# Download and unpack external projects at configure time
configure_file(CMakeLists.txt.in external-projects-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/external-projects-download)
if(result)
  message(FATAL_ERROR "CMake step for external projects failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/external-projects-download)
if(result)
  message(FATAL_ERROR "Build step for external projects failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Add boost headers to our build. As soon as we use only
# boost heap, building is not necessary.
include_directories(${CMAKE_CURRENT_BINARY_DIR}/boost-src)

# Shared library
add_library(mapper SHARED mapper.cpp)
set_target_properties(mapper PROPERTIES PUBLIC_HEADER mapper.h)

# Tests
add_executable(test test.cpp)
target_link_libraries(test gtest_main mapper)
add_test(NAME test COMMAND test)
