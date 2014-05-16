#=============================================================================
# Setup unit tests
#=============================================================================
enable_testing()
add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "Build and run all the unit tests.")

function(boost_hana_add_test name)
    add_executable(${name} EXCLUDE_FROM_ALL ${name}.cpp)
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(tests ${name})
endfunction()

boost_hana_add_test(hana)
boost_hana_add_test(integral)
boost_hana_add_test(iterable)
boost_hana_add_test(list)
boost_hana_add_test(range)
boost_hana_add_test(type)
