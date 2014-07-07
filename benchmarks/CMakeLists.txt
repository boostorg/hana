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


add_custom_target(benchmarks COMMENT "Gather data for all the benchmarks.")
configure_file(benchmark.in.rb benchmark.rb @ONLY)

# name: The name of the target referring to the benchmark. This will create
#       a file called name.csv containing the benchmark data.
#
# file: The input file to be benchmarked. The path should be relative to the
#       current directory (benchmarks/), and the file itself can be an ERB
#       template that will be evaluated prior to compilation.
#
# envs: A string of Ruby code generating an Array of Hashes to be used as
#       the environments when generating the ERB templates.
function(boost_hana_add_benchmark name file envs)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${name}.csv
        COMMAND ${RUBY_EXECUTABLE} --
                ${CMAKE_CURRENT_BINARY_DIR}/benchmark.rb    # script performing benchmarks
                ${envs}                                     # environments
                ${CMAKE_CURRENT_BINARY_DIR}/${name}.csv     # output file
                ${CMAKE_CURRENT_SOURCE_DIR}/${file}         # input file

        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/benchmark.rb    # script
                ${CMAKE_CURRENT_SOURCE_DIR}/${file}         # input file
        VERBATIM
    )
    add_custom_target(benchmark.${name}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${name}.csv)
    add_dependencies(benchmarks benchmark.${name})
endfunction()


add_custom_target(plots COMMENT "Build all the benchmark plots.")

# name: The name of the target referring to the plot. This will create
#       plots called name.time.png and name.memusg.png containing timing
#       and memory usage information, respectively.
#
# benchmarks: A list of benchmark names representing which benchmarks to plot.
#             Benchmark names should be as given to boost_hana_add_benchmark.
function(boost_hana_add_plot name) # benchmarks...
    set(benchmarks_csv "")
    foreach(bm IN LISTS ARGN)
        list(APPEND benchmarks_csv ${CMAKE_CURRENT_BINARY_DIR}/${bm}.csv)
    endforeach()

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${name}.time.png
               ${CMAKE_CURRENT_BINARY_DIR}/${name}.memusg.png

        COMMAND ${RUBY_EXECUTABLE} --
                ${CMAKE_CURRENT_SOURCE_DIR}/plot.rb             # script plotting the data
                ${CMAKE_CURRENT_BINARY_DIR}/${name}.time.png    # time output file
                ${CMAKE_CURRENT_BINARY_DIR}/${name}.memusg.png  # memory usage output file
                ${benchmarks_csv}                               # input files

        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/plot.rb             # script
                ${benchmarks_csv}                               # input files
        VERBATIM
    )
    add_custom_target(plot.${name}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${name}.time.png
                ${CMAKE_CURRENT_BINARY_DIR}/${name}.memusg.png
    )
    add_dependencies(plots plot.${name})
endfunction()


# list.permutations
set(list.permutations.env "0.upto(2).map { |n| {x: n} }")
boost_hana_add_benchmark(list.permutations.list list/permutations/list.cpp ${list.permutations.env})
boost_hana_add_benchmark(list.permutations.std_tuple list/permutations/std_tuple.cpp ${list.permutations.env})
boost_hana_add_plot(list.permutations list.permutations.list list.permutations.std_tuple)

# core.datatype
set(core.datatype.env "(0...1000).step(5).map { |n| {x: n} }")
boost_hana_add_benchmark(core.datatype.overload core/datatype/overload.cpp ${core.datatype.env})
boost_hana_add_benchmark(core.datatype.specialization core/datatype/specialization.cpp ${core.datatype.env})
boost_hana_add_plot(core.datatype core.datatype.specialization core.datatype.overload)

# list.partition
set(list.partition.env "(0...150).map { |n| { x: n } }")
boost_hana_add_benchmark(list.partition.filter list/partition/filter.cpp ${list.partition.env})
boost_hana_add_benchmark(list.partition.foldl list/partition/foldl.cpp ${list.partition.env})
boost_hana_add_plot(list.partition list.partition.foldl list.partition.filter)

# foldable.foldl
boost_hana_add_benchmark(foldable.foldl.linear1  foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 1 }}")
boost_hana_add_benchmark(foldable.foldl.linear2  foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 2 }}")
boost_hana_add_benchmark(foldable.foldl.linear4  foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 4 }}")
boost_hana_add_benchmark(foldable.foldl.linear8  foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 8 }}")
boost_hana_add_benchmark(foldable.foldl.linear16 foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 16 }}")
boost_hana_add_benchmark(foldable.foldl.linear32 foldable/foldl/linear.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 32 }}")
boost_hana_add_benchmark(foldable.foldl.log10  foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 10 }}")
boost_hana_add_benchmark(foldable.foldl.log20  foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 20 }}")
boost_hana_add_benchmark(foldable.foldl.log30  foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 30 }}")
boost_hana_add_benchmark(foldable.foldl.log40  foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 40 }}")
boost_hana_add_benchmark(foldable.foldl.log100 foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 100 }}")
boost_hana_add_benchmark(foldable.foldl.log250 foldable/foldl/log.cpp "(0...1000).step(25).map { |n| { x: n, unroll: 250 }}")

boost_hana_add_plot(foldable.foldl
        foldable.foldl.linear1 foldable.foldl.linear2 foldable.foldl.linear4 foldable.foldl.linear8 foldable.foldl.linear16 foldable.foldl.linear32
        foldable.foldl.log10 foldable.foldl.log20 foldable.foldl.log30 foldable.foldl.log40 foldable.foldl.log100 foldable.foldl.log250
)

# functor.fmap
set(functor.fmap.env "(0...1000).step(25).map { |n| { x: n } }")
boost_hana_add_benchmark(functor.fmap.list functor/fmap/list.cpp ${functor.fmap.env})
boost_hana_add_benchmark(functor.fmap.std_tuple functor/fmap/std_tuple.cpp ${functor.fmap.env})
boost_hana_add_benchmark(functor.fmap.mpl_list functor/fmap/mpl_list.cpp ${functor.fmap.env})
boost_hana_add_benchmark(functor.fmap.mpl_vector functor/fmap/mpl_vector.cpp ${functor.fmap.env})
