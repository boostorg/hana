# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup benchmarks
#=============================================================================
if (NOT ${GNUPLOT_FOUND})
    message(STATUS "Gnuplot was not found; benchmarks won't be available.")
    return()
elseif (NOT ${RUBY_FOUND})
    message(STATUS "Ruby was not found; benchmarks won't be available.")
    return()
else()
    execute_process(COMMAND ${RUBY_EXECUTABLE} -r benchcc -e ""
                    RESULT_VARIABLE BENCHCC_NOT_FOUND
                    OUTPUT_QUIET ERROR_QUIET)
    if (${BENCHCC_NOT_FOUND})
        message(STATUS "The Benchcc gem was not found; benchmarks won't be available. Use `gem install benchcc` to install it.")
        return()
    endif()
endif()

configure_file(benchmark.in.rb benchmark.rb @ONLY)
set(BOOST_HANA_BENCHMARK_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/benchmark.rb)
set(BOOST_HANA_PLOT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/plot.rb)


add_custom_target(benchmarks COMMENT "Build all the benchmark plots.")

# Creates a command which generates a file containing data from a benchmark.
#
# dataset_name:
#   The name of the dataset. This will be used for the name of the
#   file containing the dataset.
#
# cpp_file:
#   The input file to be benchmarked. The path should be relative to the
#   source directory in which the function is called. The file can be an
#   ERB template that will be evaluated prior to compilation.
#
# envs:
#   A string of Ruby code generating an Array of Hashes to be used as
#   the environments when generating the ERB templates.
function(boost_hana_add_dataset dataset_name cpp_file envs)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${dataset_name}.envs "${envs}")
    add_custom_command(OUTPUT ${dataset_name}
        COMMAND ${RUBY_EXECUTABLE} --
                ${BOOST_HANA_BENCHMARK_SCRIPT}
                ${CMAKE_CURRENT_BINARY_DIR}/${dataset_name}.envs
                ${dataset_name}
                ${CMAKE_CURRENT_SOURCE_DIR}/${cpp_file}
        DEPENDS ${BOOST_HANA_BENCHMARK_SCRIPT}
                ${CMAKE_CURRENT_SOURCE_DIR}/${cpp_file}
        IMPLICIT_DEPENDS CXX ${CMAKE_CURRENT_SOURCE_DIR}/${cpp_file}
        VERBATIM
    )
    add_custom_target(__${dataset_name} DEPENDS ${dataset_name}) # hack
endfunction()

# Creates a target representing a plot to which curves may be added later.
#
# Building a target created with this function will first build all the
# datasets it needs and then draw the plot using Gnuplot.
#
# plot_target:
#   The name of the target referring to the plot. To construct the plot, one
#   will have to build the `plot_target` target.
#
# plot_name:
#   The name of the file(s) containing the generated plot(s). This will create
#   plots called `plot_name.time.png` and `plot_name.memusg.png` containing
#   timing and memory usage information, respectively. The plots will be
#   created in the binary directory associated to the source directory where
#   the function is called.
function(boost_hana_add_plot_with_name plot_target plot_name)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.time.png
               ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.memusg.png
        COMMAND ${RUBY_EXECUTABLE} --
                ${BOOST_HANA_PLOT_SCRIPT}
                ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.time.png
                ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.memusg.png
                $<TARGET_PROPERTY:${plot_target},boost_hana_datasets>

        DEPENDS ${BOOST_HANA_PLOT_SCRIPT}

        # todo:
        # Not supported in CMake 3.0. Once this is supported, remove the
        # `add_custom_target(_${dataset_name} ...)` in `boost_hana_add_dataset`
        # and the `add_dependencies` in `boost_hana_add_curve_from_dataset`.

        # DEPENDS $<TARGET_PROPERTY:${plot_target},boost_hana_datasets>

        VERBATIM
    )
    add_custom_target(${plot_target}
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.time.png
                    ${CMAKE_CURRENT_BINARY_DIR}/${plot_name}.memusg.png
    )
    set_target_properties(${plot_target} PROPERTIES boost_hana_datasets "")
    add_dependencies(benchmarks ${plot_target})
endfunction()

function(boost_hana_add_plot plot_target)
    boost_hana_add_plot_with_name(${plot_target} ${plot_target})
endfunction()

# Add a curve on a plot.
#
# This is equivalent to creating an anonymous dataset with
# `boost_hana_add_dataset` and then calling `boost_hana_add_curve_from_dataset`.
#
# plot_name:
#   The name of the plot to which a curve is added. If no target with the
#   given name exists, it is created with `boost_hana_add_plot`.
#
# cpp_file:
#   Same as for `boost_hana_add_dataset`.
#
# envs:
#   Same as for `boost_hana_add_dataset`.
function(boost_hana_add_curve_from_source plot_name curve_name cpp_file envs)
    boost_hana_add_dataset(${plot_name}.${curve_name} ${cpp_file} "${envs}")
    boost_hana_add_curve_from_dataset(${plot_name} ${plot_name}.${curve_name})
endfunction()

# Add a curve representing a dataset to the given plot.
#
# It might be desirable to use this function when one wants a dataset to be
# shared between several plots. Otherwise, using `boost_hana_add_curve_from_source`
# is simpler.
#
# plot_name:
#   The name of the plot to which a curve is added. If no target with the
#   given name exists, it is created with `boost_hana_add_plot`.
#
# dataset_name:
#   The name of the dataset to plot.
function(boost_hana_add_curve_from_dataset plot_name dataset_name)
    if (NOT TARGET ${plot_name})
        boost_hana_add_plot(${plot_name})
    endif()
    set_property(TARGET ${plot_name} APPEND PROPERTY boost_hana_datasets ${dataset_name})
    add_dependencies(${plot_name} __${dataset_name}) # hack
endfunction()

add_subdirectory(core)
add_subdirectory(techniques)

add_subdirectory(foldable)
add_subdirectory(functor)
add_subdirectory(iterable)
add_subdirectory(list)
add_subdirectory(searchable)
