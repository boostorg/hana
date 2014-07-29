#=============================================================================
# Setup examples
#=============================================================================
enable_testing()
add_custom_target(examples ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "example.+"
    COMMENT "Build and run all the examples.")

function(boost_hana_add_example name)
    string(REGEX REPLACE "[.]" "/" source ${name})
    add_executable(example.${name} EXCLUDE_FROM_ALL ${source}.cpp)
    add_test(NAME example.${name} COMMAND example.${name})
    add_dependencies(examples example.${name})
endfunction()


file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES
     RELATIVE ${CMAKE_CURRENT_LIST_DIR}
     "*.cpp")

if (NOT Boost_FOUND)
    message(STATUS
    "The Boost library headers were not found; examples in "
    "example/ext/boost/* won't be available.")
    file(GLOB_RECURSE BOOST_HANA_EXAMPLES_DEPENDING_ON_BOOST
         RELATIVE ${CMAKE_CURRENT_LIST_DIR}
         "ext/boost/*.cpp")
    list(REMOVE_ITEM BOOST_HANA_EXAMPLE_SOURCES ${BOOST_HANA_EXAMPLES_DEPENDING_ON_BOOST})
endif()

foreach(file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    string(REGEX REPLACE "\\.cpp$" "" name ${file})
    string(REGEX REPLACE "/" "." name ${name})
    boost_hana_add_example(${name})
endforeach()
