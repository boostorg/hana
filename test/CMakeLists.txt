#=============================================================================
# Setup unit tests
#=============================================================================
enable_testing()
add_custom_target(tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
    COMMENT "Build and run all the unit tests.")

function(boost_hana_add_test name)
    string(REGEX REPLACE "[.]" "/" source ${name})
    add_executable(test.${name} EXCLUDE_FROM_ALL ${source}.cpp)
    add_test(NAME test.${name} COMMAND test.${name})
    add_dependencies(tests test.${name})
endfunction()

# Allows putting stuff that's only relevant to the unit tests in the test/
# subdirectory and including those headers as:
#   #include <test/something.hpp>
include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES
     RELATIVE ${CMAKE_CURRENT_LIST_DIR}
     "*.cpp")

if (NOT Boost_FOUND)
    message(STATUS
    "The Boost library headers were not found; unit tests in "
    "test/ext/boost/* won't be available.")
    file(GLOB_RECURSE BOOST_HANA_TESTS_DEPENDING_ON_BOOST
         RELATIVE ${CMAKE_CURRENT_LIST_DIR}
         "ext/boost/*.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${BOOST_HANA_TESTS_DEPENDING_ON_BOOST})
endif()

foreach(file IN LISTS BOOST_HANA_TEST_SOURCES)
    string(REGEX REPLACE "\\.cpp$" "" name ${file})
    string(REGEX REPLACE "/" "." name ${name})
    boost_hana_add_test(${name})
endforeach()
