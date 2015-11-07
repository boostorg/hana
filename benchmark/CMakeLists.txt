# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Required packages, gems and caveats
##############################################################################
if(NOT ${RUBY_FOUND})
    message(WARNING "Ruby >= 2.1 was not found; the benchmarks will be unavailable.")
    return()
endif()

# Check for the 'ruby-progressbar' and 'tilt' gems
execute_process(COMMAND ${RUBY_EXECUTABLE} -r ruby-progressbar -r tilt -e ""
                RESULT_VARIABLE __BOOST_HANA_MISSING_GEMS
                OUTPUT_QUIET ERROR_QUIET)
if(${__BOOST_HANA_MISSING_GEMS})
    message(WARNING
        "The 'ruby-progressbar' and/or 'tilt' gems were not found; "
        "the benchmarks will be unavailable."
        "Use `gem install ruby-progressbar tilt` to install the missing gems.")
    return()
endif()

include_directories(${Boost.Hana_SOURCE_DIR}/include)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if (MPL11_FOUND)
    include_directories(${MPL11_INCLUDE_DIR})
endif()

if (Meta_FOUND)
    include_directories(${Meta_INCLUDE_DIR})
endif()


##############################################################################
# Configure the measure.rb script
##############################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/measure.in.rb #input
               ${CMAKE_CURRENT_BINARY_DIR}/measure.rb    #output
               @ONLY)


##############################################################################
# Add the special measure file and targets
#
# The compile.benchmark.measure target is a special target that compiles
# the measure.cpp file and outputs compilation statistics.
#
# Similarly, the run.benchmark.measure target is a special target that
# compiles the measure.cpp file, runs the resulting program and then
# outputs execution time statistics.
#
# The measure.cpp file should always be left empty. The intended usage is to
# put some code in the file, benchmark it and then put that code somewhere
# else. In particular, note that the benchmarks added below use the measure
# file to perform their benchmarking, and launching the benchmarks will wipe
# anything from the `measure.cpp` file.
##############################################################################
boost_hana_append_flag(BOOST_HANA_HAS_FTEMPLATE_DEPTH -ftemplate-depth=-1)
add_executable(compile.benchmark.measure EXCLUDE_FROM_ALL measure.cpp)
set_target_properties(compile.benchmark.measure
    PROPERTIES RULE_LAUNCH_COMPILE "${CMAKE_CURRENT_BINARY_DIR}/measure.rb")
add_custom_target(run.benchmark.measure COMMAND compile.benchmark.measure)


##############################################################################
# Add the benchmarks
##############################################################################
add_custom_target(benchmarks COMMENT "Generate all the benchmarks.")

add_custom_target(travis_compiler_slug USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E echo "travis_compiler_slug: $<LOWER_CASE:${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}>")
add_custom_target(travis_config_slug USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E echo "travis_config_slug: $<LOWER_CASE:$<CONFIG>>")

file(GLOB_RECURSE BOOST_HANA_BENCHMARKS *.erb.json)
foreach(benchmark IN LISTS BOOST_HANA_BENCHMARKS)
    boost_hana_target_name_for(target ${benchmark} ".erb.json")
    get_filename_component(directory "${benchmark}" DIRECTORY)
    file(GLOB cpp_files ${directory}/*.erb.cpp)
    configure_file("${benchmark}" "${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json" @ONLY)

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${target}.json"
        COMMAND ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "json = Tilt::ERBTemplate.new('${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json').render"
            -e "File.open('${CMAKE_CURRENT_BINARY_DIR}/${target}.json', 'w') { |f| f.write(json) } "
        WORKING_DIRECTORY ${directory}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json ${cpp_files}
        VERBATIM
        COMMENT "Generating dataset for ${target}"
    )

    add_custom_target(${target} DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${target}.json")
    add_dependencies(benchmarks ${target})
endforeach()
