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

# The list of all the benchmark targets; it is filled just below.
set(BOOST_HANA_BENCHMARKS "")
file(GLOB_RECURSE BOOST_HANA_JSON_TEMPLATES *.erb.json)

foreach(_template IN LISTS BOOST_HANA_JSON_TEMPLATES)
    boost_hana_target_name_for(_target ${_template} ".erb.json")
    list(APPEND BOOST_HANA_BENCHMARKS "${_target}")
    get_filename_component(_dir "${_template}" DIRECTORY)

    file(GLOB _cpp_benchmarks ${_dir}/*.erb.cpp)

    set(_erb_json "${CMAKE_CURRENT_BINARY_DIR}/${_target}.erb.json")
    configure_file("${_template}" "${_erb_json}" @ONLY)

    set(_json "${CMAKE_CURRENT_BINARY_DIR}/${_target}.json")
    add_custom_command(OUTPUT "${_json}"
        COMMAND ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "json = Tilt::ERBTemplate.new('${_erb_json}').render"
            -e "File.open('${_json}', 'w') { |f| f.write(json) } "
        WORKING_DIRECTORY ${_dir}
        DEPENDS ${_erb_json} ${_cpp_benchmarks}
        VERBATIM
        COMMENT "Generating benchmarks for ${_target}")
    add_custom_target(${_target} DEPENDS "${_json}")
    add_dependencies(benchmarks ${_target})

    add_test(NAME ${_target}
        COMMAND ${CMAKE_COMMAND} -E env check.benchmarks=true
        ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "Tilt::ERBTemplate.new('${_erb_json}').render"
        WORKING_DIRECTORY ${_dir})
    set_tests_properties(${_target} PROPERTIES
        RESOURCE_LOCK ${CMAKE_CURRENT_SOURCE_DIR}/measure.cpp)
endforeach()


##############################################################################
# Setup targets to automatically upload the benchmarks.
#
# We have a branch named 'datasets'. This branch contains nothing, except
# directories and JSON files with the following layout:
#   debug/
#       compiler1/
#           benchmark1.json
#           ...
#           benchmarkN.json
#       ...
#       compilerN/
#           benchmark1.json
#           ...
#           benchmarkN.json
#   release/
#       ...
#
#   default/
#       ...
#
# The `debug`, `release` or `default` are what we call the `BUILD_TYPE_SLUG`
# below, and it corresponds to the CMake configuration under which the
# benchmarks were run.
#
# The `compilerN` subdirectories are what we call the `COMPILER_SLUG` below,
# ad it corresponds to the compiler used to compile the benchmarks.
#
# The `benchmarkN.json` files are those generated by the targets we set up
# just above. The 'datasets.clone' target we define below allows cloning
# the 'dataset' branch to the 'benchmark/dataset' subdirectory of the project,
# or pulling the latest revisions if the clone already exists. This directory
# is ignored by git to avoid polluting the index. Then, the 'datasets.clean'
# target allows removing the subdirectory associated to the current build type
# and compiler, and the 'datasets.copy' target allows populating that
# subdirectory with the newly built benchmarks. Finally, the 'datasets.commit'
# target allows committing the newly built benchmarks to the 'datasets' branch,
# and the 'datasets.push' target allows pushing those changes to the official
# repository. Of course, write access is needed to push the changes.
#
# Most of this is only used by our continuous integration scripts, but it is
# also possible to do it manually.
##############################################################################
if (NOT GIT_FOUND)
    message(WARNING "Git was not found; the various 'datasets.*' targets will be unavailable.")
    return()
endif()

set(DATASETS_DIR "${CMAKE_SOURCE_DIR}/benchmark/datasets")
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}" COMPILER_SLUG)
if (CMAKE_BUILD_TYPE)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_SLUG)
else()
    set(BUILD_TYPE_SLUG "default")
endif()

add_custom_target(datasets.clone
    COMMAND ${RUBY_EXECUTABLE}
    -e "if File.directory?('${DATASETS_DIR}')                                                             "
    -e "  Dir.chdir('${DATASETS_DIR}')                                                                    "
    -e "  `${GIT_EXECUTABLE} pull`                                                                        "
    -e "else                                                                                              "
    -e "  `${GIT_EXECUTABLE} clone ${BOOST_HANA_ORIGIN_URL} --depth 1 --branch=datasets ${DATASETS_DIR}`  "
    -e "end                                                                                               "
    COMMENT "Cloning the 'datasets' branch into the 'benchmark/datasets' directory"
    VERBATIM
)

add_custom_target(datasets.clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DATASETS_DIR}/${COMPILER_SLUG}
    COMMENT "Removing ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}"
    DEPENDS datasets.clone
    VERBATIM
)

add_custom_target(datasets.copy
    COMMENT "Copying the generated benchmarks from ${CMAKE_CURRENT_BINARY_DIR} to benchmark/datasets"
    DEPENDS datasets.clean
)
foreach(benchmark IN LISTS BOOST_HANA_BENCHMARKS)
    add_custom_target(${benchmark}.copy
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${benchmark}.json ${DATASETS_DIR}/${BUILD_TYPE_SLUG}/${COMPILER_SLUG}
        DEPENDS ${benchmark} datasets.clean
        VERBATIM
    )
    add_dependencies(datasets.copy ${benchmark}.copy)
endforeach()

add_custom_target(datasets.commit
    COMMAND ${GIT_EXECUTABLE} add --all .
    COMMAND ${GIT_EXECUTABLE} commit --allow-empty -m "Update benchmarks to ${BOOST_HANA_CURRENT_COMMIT} for build type '${BUILD_TYPE_SLUG}' and compiler '${COMPILER_SLUG}'"
    WORKING_DIRECTORY ${DATASETS_DIR}
    COMMENT "Updating the datasets clone with the newest benchmarks"
    DEPENDS datasets.copy
    VERBATIM
)

add_custom_target(datasets.push
    COMMAND ${GIT_EXECUTABLE} push origin datasets
    WORKING_DIRECTORY ${DATASETS_DIR}
    COMMENT "Pushing the datasets branch to GitHub"
    DEPENDS datasets.commit
    VERBATIM
)
