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


file(GLOB_RECURSE BOOST_HANA_UNIT_TEST_SOURCES
     RELATIVE ${CMAKE_CURRENT_LIST_DIR}
     "*.cpp")
foreach(file IN LISTS BOOST_HANA_UNIT_TEST_SOURCES)
    string(REGEX REPLACE "\\.cpp$" "" name ${file})
    string(REGEX REPLACE "/" "." name ${name})
    boost_hana_add_test(${name})
endforeach()
