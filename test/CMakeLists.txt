#=============================================================================
# Setup unit tests
#=============================================================================
add_custom_target(tests COMMENT "Build all the unit tests.")

function(BOOST_HANA_add_module module)
    add_custom_target(${module}
        COMMENT "Build unit tests for the ${module} module.")
    add_dependencies(tests ${module})
endfunction()

function(BOOST_HANA_add_tests module)
    if(NOT TARGET ${module})
        BOOST_HANA_add_module(${module})
    endif()

    foreach(test IN ITEMS ${ARGN})
        add_executable(${module}.${test} ${module}/${test}.cpp)
        add_dependencies(${module} ${module}.${test})
    endforeach()
endfunction()


