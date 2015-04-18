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
boost_hana_add_executable(compile.benchmark.measure EXCLUDE_FROM_ALL measure.cpp)
set_target_properties(compile.benchmark.measure
    PROPERTIES RULE_LAUNCH_COMPILE "${CMAKE_CURRENT_BINARY_DIR}/measure.rb")
add_custom_target(run.benchmark.measure COMMAND compile.benchmark.measure)


##############################################################################
# Add the benchmarks
##############################################################################
add_custom_target(benchmarks COMMENT "Generate all the benchmarks.")

file(GLOB BOOST_HANA_BENCHMARKS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.erb.json)
foreach(benchmark IN LISTS BOOST_HANA_BENCHMARKS)
    string(REPLACE ".erb.json" "" benchmark "${benchmark}")
    set(template_file "${CMAKE_CURRENT_SOURCE_DIR}/${benchmark}.erb.json")
    set(output_file   "${CMAKE_CURRENT_BINARY_DIR}/${benchmark}.json")
    add_custom_command(OUTPUT "${output_file}"
        COMMAND ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "json = Tilt::ERBTemplate.new('${template_file}').render"
            -e "File.open('${output_file}', 'w') { |f| f.write(json) }"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS "${template_file}"
        VERBATIM)
    add_custom_target(${benchmark} DEPENDS "${output_file}")
    add_dependencies(benchmarks ${benchmark})
endforeach()
