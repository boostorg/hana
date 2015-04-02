# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# CMake module to perform compile-time and run-time benchmarks.

# Usage and dependencies
# ----------------------
# To use this module, simply make sure this file is in your CMake module path
# and then include it:
#
#   include(Benchmarks)
#
# This module has the following dependencies:
#   - Ruby >= 2.1
#     For driving the benchmarks
#
#   - the Benchcc gem
#     For driving the benchmarks
#
#   - Gnuplot
#     For drawing the plots
#
# When the module is included, it checks for the above dependencies. If they
# are not available, a STATUS message is printed, the BENCHMARK_AVAILABLE
# variable is set to false and and we immediately return to the calling CMake
# file. Once the required dependencies are installed properly, you can use the
# functions of the public API which are documented below.
#
# Also note that this module will only work with Clang for the moment.

# Tutorial
# --------
# At a high level, the module works by using the ERB templating system
# [http://en.wikipedia.org/wiki/ERuby] to generate C++ files, which we then
# compile and optionally run to gather statistics. In a bit more details,
# here's how you would go about performing a basic compile-time benchmark.
# First, you write a C++ file (call it "std_tuple.cpp") that can contain ERB
# markup:
#
#   #include <tuple>
#
#   template <int i> struct x { };
#   int main() {
#       std::tuple<
#           <%= (1..10).to_a.map { |i| "x<#{i}>" }.join(', ') %>
#       > ts;
#   }
#
# Code contained inside <%= ... %> is just normal Ruby code. When the file
# will be rendered, the contents of <%= ... %> will be replaced with the
# result of evaluating this Ruby code:
#
#   #include <tuple>
#
#   template <int i> struct x { };
#   int main() {
#       std::tuple<
#           x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>
#       > ts;
#   }
#
# Ok, so we can generate files, but so far it's pretty much equivalent to
# using some nasty preprocessor sorcery. However, we can also pass information
# to our C++/ERB file when we render it, which makes it like a 'parameterized
# file':
#
#   #include <tuple>
#
#   template <int i> struct x { };
#   int main() {
#       std::tuple<
#           <%= (1..input_size).to_a.map { |i| "x<#{i}>" }.join(', ') %>
#       > ts;
#   }
#
# Notice that inside the Ruby code, we have replaced `10` by `input_size`.
# This is a variable that should be made available to the ERB/C++ file when
# it is rendered. We will say that such variables are part of the 'environment'.
# As far as we're concerned, an environment is simply represented as a Ruby
# Hash (basically a std::unordered_map).
#
# This module makes it possible to bundle ERB/C++ files with sequences of
# environments as CMake targets. We use the name 'data set' for such targets.
# So a data set is just a ERB/C++ file along with a sequence of environments
# to render the file with. When we build the CMake target representing that
# data set, it renders the file for each environment we provided and gathers
# statistics about that file. The statistics that are currently supported are
# the compilation time, the execution time of the generated program and the
# compile-time memory usage. We call these statistics 'features'. To create
# a data set in your CMake file, simply use the Benchmark_add_dataset function
# provided by this module:
#
#   Benchmark_add_dataset(std_tuple_dataset
#       FILE "std_tuple.cpp"
#       FEATURES "COMPILATION_TIME" "MEMORY_USAGE"
#       ENV "0.upto(100).map { |n|
#           {input_size: n} # <- This is an environment
#       }"
#   )
#
# The function is described in the below module in more details. Basically,
# this will create a target named "std_tuple_dataset", which, when built, will
# render the "std_tuple.cpp" ERB/C++ file for each provided environment. For
# each rendered file, it will measure the compilation time and memory usage at
# compile-time. Then, once all is done, it will write the data to a csv file.
#
# So far so good, but we'd like to visualize our data. This module allows data
# sets to be plotted. Basically, you create a CMake target representing a plot
# with the Benchmark_add_plot function:
#
#   Benchmark_add_plot(tuple_plot
#       TITLE "Default constructing a tuple"
#       FEATURE "COMPILATION_TIME"
#       OUTPUT "tuple_plot.png"
#   )
#
# This will create a CMake target which, when built, will draw a plot to
# "tuple_plot.png". But the plot has nothing to draw so far. For this,
# we must associate our data set to the plot by creating a curve:
#
#   Benchmark_add_curve(
#       PLOT tuple_plot
#       TITLE "std::tuple"
#       DATASET std_tuple_dataset
#   )
#
# Now, whenever we build the 'tuple_plot' target, the data set will be gathered.
# Of course, the data set will only be gathered if it is outdated, which is
# important for large benchmarks. It is possible to add more than one curve to
# a plot by simply calling Benchmark_add_curve with another data set.
#
# Ok, so that's it for the basics. Note that there are several other tricks
# like creating a plot with curves and data sets in a single command, but
# this is documented in the reference below.

# Runtime benchmarks
# ------------------
# Performing runtime benchmarks require the file that is being benchmarked to
# follow a couple of rules. Basically, the benchmarked code has to output some
# statistics about its execution so they can be picked up by this module. To do
# this, a special "benchmark.hpp" header is made available to the benchmarked
# code by modifying the include path of the compiler. The header can be
# included as follows in the benchmarked code:
#
#   #include "benchmark.hpp"
#
#   int main() {
#       ...
#   }
#
# This header provides a function called `boost::hana::benchmark::measure`,
# which is used to measure the execution time of a piece of code. When using
# the EXECUTION_TIME feature, only the execution time of the `measure`d piece
# of code is actually taken into account:
#
#   #include "benchmark.hpp"
#
#   int main() {
#       // possibly some initialization
#
#       boost::hana::benchmark::measure([]{
#           // only the code in there is taken into account for execution time
#       });
#   }
#
# Specifically, `measure` takes a nullary function object, calls it several
# times and then looks at the average execution time for that function. Note
# that the function should not perform side effects, since it is called several
# times and we expect each call to do the same amount of work.
#
# Internally, `measure` will actually use `std::cout` to report the execution
# time of the piece of code. Then, this module will run the program, look at
# the standard output and interpret it as the execution time. Hence, the
# program should not output any other information to stdout, because that
# would confuse the module.

# Globally-configurable options
# -----------------------------
# This module can be globally configured through several options. Those
# options may either be defined before including the module or edited in
# the cache.
#
#   BENCHMARK_COMPILATION_TIMEOUT
# The default timeout (in seconds) when compiling a file. If compiling
# the file takes longer than that, compilation is simply aborted and
# gathering of the benchmark data is stopped there. This defaults to
# 30 seconds.
#
#   BENCHMARK_EXECUTION_TIMEOUT
# The default timeout (in seconds) when executing a program. If executing
# a generated program takes longer than that, execution is simply aborted
# and gathering of the benchmark data is stopped there. This defaults to
# 30 seconds.

# Global targets and variables created by this module
# ---------------------------------------------------
#   BENCHMARK_AVAILABLE
# A boolean representing whether the functionality provided by this module is
# available. Reasons for the module being unavailable include missing
# dependencies and other errors. Note that if this variable is set to false,
# then functions, targets and variables usually created by this module are
# not defined.
#
#   BENCHMARK_ALL_PLOTS
# Target used to draw all the plots. Note that plots that are up to date won't
# be redrawn.
#
#   BENCHMARK_ALL_DATASETS
# Target used to gather all the data sets. Note that data sets that are up to
# date won't be gathered again.

# Public API
# ----------
# See below before each function definition.

##############################################################################
# Required modules and initial setup
##############################################################################
cmake_policy(VERSION 3.1.1)
include(CMakeParseArguments)

set(BENCHMARK_AVAILABLE false)

# check for Gnuplot
find_package(Gnuplot)
if(NOT ${GNUPLOT_FOUND})
    message(WARNING
        "Gnuplot was not found; the Benchmarks module can't be used.")
    return()
endif()

# check for Ruby
find_package(Ruby 2.1)
if(NOT ${RUBY_FOUND})
    message(WARNING
        "Ruby 2.1+ was not found; the Benchmarks module can't be used.")
    return()
endif()

# check for Benchcc
execute_process(COMMAND ${RUBY_EXECUTABLE} -r benchcc -e ""
                RESULT_VARIABLE __BENCHMARK_BENCHCC_NOT_FOUND
                OUTPUT_QUIET ERROR_QUIET)
if(${__BENCHMARK_BENCHCC_NOT_FOUND})
    message(WARNING
        "The Benchcc gem was not found; the Benchmarks module can't "
        "be used. Use `gem install benchcc` to install it.")
    return()
endif()

# setup support directories
set(__BENCHMARK_SUPPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Benchmarks_support")
file(MAKE_DIRECTORY
    "${__BENCHMARK_SUPPORT_DIR}"
    "${__BENCHMARK_SUPPORT_DIR}/include"
    "${__BENCHMARK_SUPPORT_DIR}/envs")

# create the benchmark.hpp header if needed
if (${CMAKE_CURRENT_LIST_FILE} IS_NEWER_THAN "${__BENCHMARK_SUPPORT_DIR}/include/benchmark.hpp")
    file(WRITE "${__BENCHMARK_SUPPORT_DIR}/include/benchmark.hpp"
        "/*                                                                              \n"
        "@copyright Louis Dionne 2015                                                    \n"
        "Distributed under the Boost Software License, Version 1.0.                      \n"
        "(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)  \n"
        " */                                                                             \n"
        "                                                                                \n"
        "#ifndef BOOST_HANA_BENCHMARK_MEASURE_HPP                                        \n"
        "#define BOOST_HANA_BENCHMARK_MEASURE_HPP                                        \n"
        "                                                                                \n"
        "#include <chrono>                                                               \n"
        "#include <iostream>                                                             \n"
        "#include <thread>                                                               \n"
        "                                                                                \n"
        "                                                                                \n"
        "namespace boost { namespace hana { namespace benchmark {                        \n"
        "    template <int i>                                                            \n"
        "    struct object {                                                             \n"
        "        constexpr object() = default;                                           \n"
        "                                                                                \n"
        "        object(object const&) {                                                 \n"
        "            std::this_thread::sleep_for(std::chrono::nanoseconds(1));           \n"
        "        }                                                                       \n"
        "    };                                                                          \n"
        "                                                                                \n"
        "    auto measure = [](auto f) {                                                 \n"
        "        constexpr auto repetitions = 1000ull;                                   \n"
        "        auto start = std::chrono::steady_clock::now();                          \n"
        "        for (auto i = repetitions; i > 0; --i) {                                \n"
        "            f();                                                                \n"
        "        }                                                                       \n"
        "        auto stop = std::chrono::steady_clock::now();                           \n"
        "                                                                                \n"
        "        auto time = std::chrono::duration_cast<std::chrono::microseconds>(      \n"
        "            (stop - start) / repetitions                                        \n"
        "        );                                                                      \n"
        "        std::cout << \"execution time: \" << time.count() << std::endl;         \n"
        "    };                                                                          \n"
        "}}}                                                                             \n"
        "                                                                                \n"
        "#endif                                                                          \n"
    )
endif()

set(BENCHMARK_AVAILABLE true)

##############################################################################
# Configurable module-wide options and global targets
##############################################################################
if(NOT DEFINED BENCHMARK_COMPILATION_TIMEOUT)
    set(BENCHMARK_COMPILATION_TIMEOUT 30 CACHE STRING
        "Default timeout when benchmarking the compilation time of a file (in sec).")
endif()

if(NOT DEFINED BENCHMARK_EXECUTION_TIMEOUT)
    set(BENCHMARK_EXECUTION_TIMEOUT 30 CACHE STRING
        "Default timeout when benchmarking the execution time of a generated program (in sec).")
endif()

add_custom_target(BENCHMARK_ALL_PLOTS COMMENT "Drawing all out-of-date plots.")
add_custom_target(BENCHMARK_ALL_DATASETS COMMENT "Gathering all out-of-date data sets.")


##############################################################################
# Module API
##############################################################################
#   Benchmark_add_dataset(<target name>
#       FILE <source file>
#       FEATURES <feature1> [features...]
#       ENV <ERB environments>
#       [OUTPUT <file name>]
#       [COMPILATION_TIMEOUT <duration>]
#       [EXECUTION_TIMEOUT <duration>]
#   )
#
# Create a named target for gathering the specified benchmark data from
# a source file.
#
#   <target name>
# The name of the target used to regenerate this data set.
#
#   FILE <source file>
# The input file to be benchmarked. The path should be relative to the
# source directory in which the function is called. The file can be an
# ERB template that will be evaluated prior to compilation.
#
#   FEATURES <feature1> [features...]
# A list of features to measure. At least one feature must be measured.
# Supported features are "COMPILATION_TIME", "EXECUTION_TIME" and "MEMORY_USAGE".
#
#   ENV <ERB environments>
# A string of Ruby code generating an Array of Hashes to be used as the
# environments when generating the source files that are then benchmarked.
# Be careful about quoting; double quotes must be escaped. Also, no ';'
# may appear in that string since CMake will do weird stuff to it.
#
#   [OUTPUT <file name>]
# The file name of the generated data set. That file will be created in the
# binary directory of the source directory where the function is called. If
# left unspecified, the file name defaults to "<target name>.csv".
#
#   [COMPILATION_TIMEOUT <duration>]
# The compilation timeout (in seconds) when gathering data for this data set.
# If compiling the file takes longer than <duration> seconds, compilation is
# simply aborted and the data set stops there. This defaults to the global
# configuration option BENCHMARK_COMPILATION_TIMEOUT.
#
#   [EXECUTION_TIMEOUT <duration>]
# The execution timeout (in seconds) when gathering data for this data set.
# If executing a generated program takes longer than <duration> seconds,
# execution is simply aborted and the data set stops there. This defaults
# to the global configuration option BENCHMARK_EXECUTION_TIMEOUT.
#
#   [COMPILER_FLAGS <flags>...]
# A list of flags to pass to the compiler. If this is not given, it defaults
# to the concatenation of the COMPILE_OPTIONS, COMPILE_DEFINITIONS and
# INCLUDE_DIRECTORIES properties of the directory calling the function.
#
#   [ADDITIONAL_COMPILER_FLAGS <flags>...]
# A list of additional flags to pass to the compiler. Use this when you do
# not want to override all the COMPILER_FLAGS, but only add some flags.
function(Benchmark_add_dataset target_name)
    # Parse arguments
    cmake_parse_arguments(my ""                                     # options
           "FILE;OUTPUT;ENV;COMPILATION_TIMEOUT;EXECUTION_TIMEOUT"  # 1 value args
           "FEATURES;COMPILER_FLAGS;ADDITIONAL_COMPILER_FLAGS"      # multi-valued args
           ${ARGN})

    # Sanitize arguments
    if(NOT my_FILE OR NOT my_FEATURES OR NOT my_ENV)
        message(FATAL_ERROR "Missing FILE, FEATURES or ENV argument")
    endif()
    __Benchmark_validate_features_impl(${my_FEATURES})

    # Actual processing
    if(NOT my_OUTPUT)
        set(my_OUTPUT "${target_name}.csv")
    endif()
    if(NOT my_COMPILATION_TIMEOUT)
        set(my_COMPILATION_TIMEOUT ${BENCHMARK_COMPILATION_TIMEOUT})
    endif()
    if(NOT my_EXECUTION_TIMEOUT)
        set(my_EXECUTION_TIMEOUT ${BENCHMARK_EXECUTION_TIMEOUT})
    endif()
    if(NOT my_ADDITIONAL_COMPILER_FLAGS)
        set(my_ADDITIONAL_COMPILER_FLAGS "")
    endif()
    if(NOT my_COMPILER_FLAGS)
        get_directory_property(_options DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_OPTIONS)
        get_directory_property(_defs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)
        get_directory_property(_includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} INCLUDE_DIRECTORIES)
        set(my_COMPILER_FLAGS ${_options} ${_defs})
        # TODO: Make this portable.
        foreach(_dir IN LISTS _includes)
            list(APPEND my_COMPILER_FLAGS "-I${_dir}")
        endforeach()
    endif()

    # TODO: Make this portable.
    list(APPEND my_COMPILER_FLAGS
        "-I${__BENCHMARK_SUPPORT_DIR}/include"
        ${my_ADDITIONAL_COMPILER_FLAGS}
    )

    # We need to write the environment to a file because they are often
    # on several lines, which messes up when it appears inside a Makefile.
    set(_env_file "${__BENCHMARK_SUPPORT_DIR}/envs/${target_name}")
    file(WRITE ${_env_file} "${my_ENV}")
    add_custom_command(
        OUTPUT "${my_OUTPUT}"
        COMMAND ${RUBY_EXECUTABLE}
        -e "require 'benchcc'                                                           "
        -e "require 'pathname'                                                          "
        -e "                                                                            "
        -e "csv = Benchcc::benchmark(                                                   "
        -e "  erb_file: '${my_FILE}',                                                   "
        -e "  environments: eval(File.read('${_env_file}')),                            "
        -e "  compilation_timeout: ${my_COMPILATION_TIMEOUT},                           "
        -e "  execution_timeout: ${my_EXECUTION_TIMEOUT},                               "
        -e "  evaluate_erb_relative_to: '${CMAKE_CURRENT_SOURCE_DIR}',                  "
        -e "  features: '${my_FEATURES}'.split(';').map { |f| f.downcase.to_sym },      "
        -e "  compiler_executable: '${CMAKE_CXX_COMPILER}',                             "
        -e "  compiler_id: '${CMAKE_CXX_COMPILER_ID}',                                  "
        -e "  compiler_options: '${my_COMPILER_FLAGS}'.split(';')                       "
        -e ")                                                                           "
        -e "                                                                            "
        -e "OUTPUT_FILE = Pathname.new('${my_OUTPUT}')                                  "
        -e "OUTPUT_FILE.dirname.mkpath                                                  "
        -e "OUTPUT_FILE.write(csv)                                                      "
        DEPENDS "${my_FILE}" "${_env_file}"
        VERBATIM
        COMMENT "Gathering data set at ${my_OUTPUT} from ${my_FILE}.")
    add_custom_target(${target_name} DEPENDS ${my_OUTPUT})
    set_target_properties(${target_name} PROPERTIES
        features "${my_FEATURES}"
        output "${my_OUTPUT}")
    add_dependencies(BENCHMARK_ALL_DATASETS ${target_name})
endfunction()

#   Benchmark_add_curve(
#       PLOT <plot target>
#       TITLE <curve title>
#       DATASET <data set target>
#   )
# Add a curve to an existing plot. The data on the curve is that of the
# specified data set.
#
#   Benchmark_add_curve(
#       PLOT <plot target>
#       TITLE <curve title>
#       <data set specification>
#   )
# Create a data set inline and add a curve to an existing plot, with the
# curve's data being that of the inline data set.
#
#   PLOT <plot target>
# The name of the plot target to which a curve is added.
#
#   TITLE <curve title>
# A string representing the title of the curve on the plot.
#
#   DATASET <data set target>
# The data set from which data is taken to draw the curve. The data set must
# possess the feature that is being plotted.
#
#   <data set specification>
# Instead of specifying the name of an existing data set, it is also possible
# to create a data set inline that will be used only for this curve. To do so,
# specify exactly the same arguments as when calling Benchmark_add_dataset,
# except for the <target name> and FEATURES arguments, which must be omitted.
# An anonymous data set will be created with the provided arguments and that
# data set will be used for the curve being created.
function(Benchmark_add_curve)
    # Parse arguments
    cmake_parse_arguments(my ""                    # options
                             "PLOT;TITLE;DATASET"  # 1 value args
                             ""                    # multi-valued args
                             ${ARGN})

    # Sanitize arguments
    if(NOT my_PLOT OR NOT my_TITLE)
        message(FATAL_ERROR "Missing PLOT or TITLE argument")

    elseif(NOT TARGET ${my_PLOT})
        message(FATAL_ERROR "${my_PLOT} does not denote an existing plot "
                            "created with Benchmark_add_plot")

    elseif(NOT my_DATASET AND NOT my_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Exactly one of DATASET and <data set specification> "
                            "must be provided; none was given.")

    elseif(my_DATASET AND my_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "You seem to be mixing up the two signatures of Benchmark_add_curve; "
            "DATASET was provided but there are also trailing arguments, which "
            "are assumed to be part of a <data set specification>.")

    elseif(my_DATASET AND NOT TARGET ${my_DATASET})
        message(FATAL_ERROR "${my_DATASET} does not denote an existing dataset "
                            "created with Benchmark_add_dataset")

    elseif(my_DATASET)
        get_target_property(_ds_features ${my_DATASET} features)
        get_target_property(_plot_feature ${my_PLOT} feature)
        __Benchmark_list_contains_impl(_ds_features ${_plot_feature} _ds_provides_feature)
        if(NOT _ds_provides_feature)
            message(FATAL_ERROR
                "Curve ${my_TITLE} requires data set ${my_DATASET}, which does "
                "not support the feature ${_plot_feature} required by the plot "
                "${my_PLOT}.")
        endif()
    endif()

    # Actual processing
    if(my_DATASET)
        get_target_property(_dataset_file ${my_DATASET} output)
        set_property(TARGET ${my_PLOT} APPEND PROPERTY curve_titles "${my_TITLE}")
        set_property(TARGET ${my_PLOT} APPEND PROPERTY dataset_files "${_dataset_file}")
        add_dependencies(${my_PLOT} ${my_DATASET})
    else()
        get_target_property(_feature ${my_PLOT} "feature")
        get_target_property(_id ${my_PLOT} "unique_id")
        Benchmark_add_dataset("${my_PLOT}.ds${_id}" FEATURES "${_feature}" ${my_UNPARSED_ARGUMENTS})
        Benchmark_add_curve(PLOT ${my_PLOT} TITLE "${my_TITLE}" DATASET "${my_PLOT}.ds${_id}")
        math(EXPR _id "${_id} + 1")
        set_target_properties(${my_PLOT} PROPERTIES "unique_id" "${_id}")
    endif()
endfunction()

#   Benchmark_add_curves(
#       PLOT <plot target>
#       [CURVE <curve specification>]...
#   )
#
# Add several curves to an existing plot. Specifically, this is equivalent to
# calling Benchmark_add_curve for each CURVE argument, with the PLOT argument
# implicitly being the given <plot target>. This allows adding several curves
# to the same plot without having to re-type the plot's target each time.
#
#   PLOT <plot target>
# The name of the plot target to which the curves are added.
#
#   CURVE <curve specification>
# The arguments that would normally be used when calling Benchmark_add_curve.
# Note, however, that the PLOT argument is implicitly set to the given
# <plot target>. Also note that CURVE arguments must appear last.
function(Benchmark_add_curves)
    # Parse arguments (trailing CURVE are handled manually)
    cmake_parse_arguments(my ""     # options
                             "PLOT" # 1 value args
                             ""     # multi-valued args
                             ${ARGN})

    if(NOT my_PLOT)
        message(FATAL_ERROR "Missing PLOT argument.")
    endif()

    while(my_UNPARSED_ARGUMENTS)
        # drop the leading CURVE keyword
        __Benchmark_list_drop_impl(my_UNPARSED_ARGUMENTS 1)
        set(_curve "${my_UNPARSED_ARGUMENTS}")

        list(FIND my_UNPARSED_ARGUMENTS "CURVE" _next_index)
        if(${_next_index} EQUAL -1)
            list(LENGTH my_UNPARSED_ARGUMENTS _next_index)
        endif()
        __Benchmark_list_take_impl(_curve ${_next_index})
        __Benchmark_list_drop_impl(my_UNPARSED_ARGUMENTS ${_next_index})

        Benchmark_add_curve(PLOT ${my_PLOT} ${_curve})
    endwhile()
endfunction()

#   Benchmark_add_plot(<target name>
#       TITLE <plot title>
#       FEATURE <feature>
#       [OUTPUT <file name>]
#       [CURVE <curve specification>]...
#   )
#
# Creates a target representing a plot to which curves may be added later.
# Building the target created with this function will first build all the
# datasets it needs and then draw the plot.
#
#   <target name>
# The name of the target used to generate this plot.
#
#   [TITLE <plot title>]
# A string used as the title for the plot. If this is not specified, it
# defaults to the <target name>
#
#   FEATURE <feature>
# The feature drawn on the Y-axis.
#
#   [OUTPUT <file name>]
# The file name of the generated plot. The plot will be created in the binary
# directory of the source directory where the function is called. If left
# unspecified, the file name for the plot defaults to "<target name>.png".
#
#   [CURVE <curve specification>]...
# The same as when calling Benchmark_add_curves. Also note that CURVE
# arguments must appear last in a call to Benchmark_add_plot.
function(Benchmark_add_plot target_name)
    # Parse arguments (CURVE is handled manually)
    __Benchmark_list_partition_impl(ARGN "CURVE" _args _curves)
    cmake_parse_arguments(my ""                     # options
                             "TITLE;FEATURE;OUTPUT" # 1 value args
                             ""                     # multi-valued args
                             ${_args})

    # Sanitize arguments
    if(NOT my_FEATURE)
        message(FATAL_ERROR "Missing FEATURE argument")
    endif()
    __Benchmark_validate_features_impl(${my_FEATURE})

    # Actual processing
    if(NOT my_TITLE)
        set(my_TITLE "${target_name}")
    endif()
    if(NOT my_OUTPUT)
        set(my_OUTPUT "${target_name}.png")
    endif()

    add_custom_target(${target_name} COMMENT "Drawing plot at ${my_OUTPUT}.")
    set_target_properties(${target_name} PROPERTIES
        curve_titles ""
        dataset_files ""
        unique_id 0
        feature "${my_FEATURE}")
    add_dependencies(BENCHMARK_ALL_PLOTS ${target_name})

    add_custom_command(
        TARGET ${target_name}
        COMMAND ${RUBY_EXECUTABLE} -r benchcc
            -e "require 'benchcc'                                                       "
            -e "                                                                        "
            -e "titles = '$<TARGET_PROPERTY:${target_name},curve_titles>'.split(';')    "
            -e "datasets = '$<TARGET_PROPERTY:${target_name},dataset_files>'.split(';') "
            -e "curves = titles.zip(datasets).map { |t, f| {title: t, input: f} }       "
            -e "Benchcc::plot('${my_TITLE}', '${my_OUTPUT}', curves,                    "
            -e "    y_feature: '${my_FEATURE}'.downcase                                 "
            -e ")                                                                       "
        VERBATIM)

    Benchmark_add_curves(PLOT ${target_name} ${_curves})
endfunction()

##############################################################################
# Implementation details
##############################################################################
function(__Benchmark_validate_features_impl)
    foreach(f IN LISTS ARGN)
        if(NOT "${f}" MATCHES "MEMORY_USAGE|COMPILATION_TIME|EXECUTION_TIME")
            message(FATAL_ERROR
                "Invalid feature ${f}. Available features are MEMORY_USAGE, "
                "COMPILATION_TIME and EXECUTION_TIME.")
        endif()
    endforeach()
endfunction()

# Returns whether some value is contained in a list.
#
#   __Benchmark_list_contains_impl(<list> <value> <output variable>)
#
# Specifically, <output variable> is TRUE if <value> can be found in <list>,
# and FALSE otherwise.
function(__Benchmark_list_contains_impl list value output_variable)
    list(FIND ${list} "${value}" _res)
    if (_res EQUAL -1)
        set(${output_variable} FALSE PARENT_SCOPE)
    else()
        set(${output_variable} TRUE PARENT_SCOPE)
    endif()
endfunction()

# Slice a list at the given indices.
#
#   __Benchmark_list_slice_impl(<list> <from> <to> [step])
#
# Specifically, modifies <list> in place so it contains the element at
# indices in the half-open interval [<from>, <to>) with the specified <step>.
# If <step> is left unspecified, it defaults to 1.
function(__Benchmark_list_slice_impl list from to)
    if(${to} EQUAL ${from})
        set(${list} "" PARENT_SCOPE)
        return()
    elseif(${from} GREATER ${to})
        message(FATAL_ERROR "Invalid slice with from > to (${from} > ${to}).")
    endif()

    list(LENGTH ${list} _length)
    if(${from} GREATER ${_length})
        message(FATAL_ERROR "Invalid slice with from > length (${from} > ${length}).")
    endif()

    math(EXPR to "${to} - 1")
    set(_res_list_slice)
    foreach(i RANGE ${from} ${to} ${ARGN})
        list(GET ${list} ${i} _x)
        list(APPEND _res_list_slice "${_x}")
    endforeach()
    set(${list} "${_res_list_slice}" PARENT_SCOPE)
endfunction()

# Take the first <n> elements in a list.
#
#   __Benchmark_list_take_impl(<list> <n>)
#
# Specifically, modifies <list> in place so it contains its first <n> elements
# only, or all the elements if <n> is greater than the length of <list>.
function(__Benchmark_list_take_impl list n)
    list(LENGTH ${list} _length)
    if(${n} GREATER ${_length})
        set(n ${_length})
    endif()

    set(_res "${${list}}")
    __Benchmark_list_slice_impl(_res 0 ${n})
    set(${list} "${_res}" PARENT_SCOPE)
endfunction()

# Drop the first <n> elements in a list.
#
#   __Benchmark_list_drop_impl(<list> <n>)
#
# Specifically, removes the first <n> elements of the <list> in place, or
# all the elements if <n> is greater than the length of <list>.
function(__Benchmark_list_drop_impl list n)
    list(LENGTH ${list} _length)
    if(${n} GREATER ${_length})
        set(n ${_length})
    endif()

    set(_res "${${list}}")
    __Benchmark_list_slice_impl(_res ${n} ${_length})
    set(${list} "${_res}" PARENT_SCOPE)
endfunction()

# Partition a list into two sublists.
#
#   __Benchmark_list_partition_impl(<list> <value> <left> <right>)
#
# Specifically, partition <list> into a list of values before the first
# occurrence of <value> and after (and including) its first occurence.
# The values before are put into the <left> list, and those after in the
# <right> list. Comparison of values is done with STREQUAL.
macro(__Benchmark_list_partition_impl list value left right)
    set(__Benchmark_list_partition_impl_left TRUE)
    foreach(x IN LISTS ${list})
        if(${__Benchmark_list_partition_impl_left})
            if("${x}" STREQUAL "${value}")
                set(__Benchmark_list_partition_impl_left FALSE)
            else()
                list(APPEND ${left} "${x}")
            endif()
        endif()

        if(NOT ${__Benchmark_list_partition_impl_left})
            list(APPEND ${right} "${x}")
        endif()
    endforeach()
    unset(__Benchmark_list_partition_impl_left)
endmacro()