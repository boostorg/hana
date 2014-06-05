#=============================================================================
# Setup examples
#=============================================================================
add_custom_target(examples
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "example.+"
    COMMENT "Build and run all the examples.")

function(boost_hana_add_example name)
    string(REGEX REPLACE "[.]" "/" source ${name})
    add_executable(example.${name} EXCLUDE_FROM_ALL ${source}.cpp)
    add_test(NAME example.${name} COMMAND example.${name})
    add_dependencies(examples example.${name})
endfunction()

boost_hana_add_example(functional.always)
boost_hana_add_example(functional.apply)
boost_hana_add_example(functional.arg)
boost_hana_add_example(functional.compose)
boost_hana_add_example(functional.curry)
boost_hana_add_example(functional.fbind)
boost_hana_add_example(functional.fix)
boost_hana_add_example(functional.flip)
boost_hana_add_example(functional.id)
boost_hana_add_example(functional.lockstep)
boost_hana_add_example(functional.on)
boost_hana_add_example(functional.partial)
boost_hana_add_example(functional.placeholder)

boost_hana_add_example(fusion.all)
boost_hana_add_example(fusion.any)
boost_hana_add_example(fusion.concat)
boost_hana_add_example(fusion.count)
boost_hana_add_example(fusion.filter)
boost_hana_add_example(fusion.find)
boost_hana_add_example(fusion.foldl)
boost_hana_add_example(fusion.foldr)
boost_hana_add_example(fusion.none)
boost_hana_add_example(fusion.reverse)
boost_hana_add_example(fusion.zip)
boost_hana_add_example(fusion.zip_with)

boost_hana_add_example(mpl.any)
boost_hana_add_example(mpl.count)
boost_hana_add_example(mpl.find)
boost_hana_add_example(mpl.fmap)
boost_hana_add_example(mpl.foldl)
boost_hana_add_example(mpl.foldr)
