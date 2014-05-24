#=============================================================================
# Setup examples
#=============================================================================
add_custom_target(examples COMMENT "Build all the examples.")

function(boost_hana_add_example name)
    string(REGEX REPLACE "[.]" "/" source ${name})
    add_executable(${name} EXCLUDE_FROM_ALL ${source}.cpp)
    add_custom_target(_${name} ${name} DEPENDS ${name})
    add_dependencies(examples _${name})
endfunction()

boost_hana_add_example(mpl.any)
boost_hana_add_example(mpl.fmap)

boost_hana_add_example(fusion.any)
