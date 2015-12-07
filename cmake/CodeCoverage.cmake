# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "Code coverage requires CMAKE_BUILD_TYPE=Debug.")
endif()

find_program(_lcov lcov)
if (NOT _lcov)
  message(FATAL_ERROR "Cannot find lcov")
endif()

find_program(_genhtml genhtml)
if (NOT _genhtml)
  message(FATAL_ERROR "Cannot find genhtml")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

function(setup_coverage_target target_name output_dir capture_dirs source_dirs)
    file(MAKE_DIRECTORY ${output_dir})

    set(_capture_directories "")
    foreach(dir ${capture_dirs})
        list(APPEND CAPTURE_DIRS "-d;${dir}")
    endforeach()

    set(_extract_directories "")
    foreach(dir ${source_dirs})
        list(APPEND EXTRACT_DIRS "'${dir}/*'")
    endforeach()

    add_custom_target(${target_name}
        COMMAND ${_lcov} --capture ${_capture_directories} -o test_coverage.info
        COMMAND ${_lcov} --extract test_coverage.info ${_extract_directories} -o test_coverage.info
        COMMAND ${_genhtml} --demangle-cpp test_coverage.info -o html
        COMMAND ${CMAKE_COMMAND} -E remove test_coverage.info
        WORKING_DIRECTORY ${output_dir}
        COMMENT "Generating test coverage results"
    )
endfunction()
