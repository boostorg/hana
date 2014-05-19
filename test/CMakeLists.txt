#=============================================================================
# Setup unit tests
#=============================================================================
enable_testing()
add_custom_target(tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "Build and run all the unit tests.")

function(boost_hana_add_test name)
    string(REGEX REPLACE "[.]" "/" source ${name})
    add_executable(${name} EXCLUDE_FROM_ALL ${source}.cpp)
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(tests ${name})
endfunction()

boost_hana_add_test(detail.comparable_from_iterable)
boost_hana_add_test(detail.foldable_from_iterable)
boost_hana_add_test(foldable)
boost_hana_add_test(hana)
boost_hana_add_test(integral)
boost_hana_add_test(iterable)
boost_hana_add_test(list)
boost_hana_add_test(logical)
boost_hana_add_test(misc.adl)
boost_hana_add_test(range)
boost_hana_add_test(type.mpl_equivalent)
boost_hana_add_test(type.traits)
