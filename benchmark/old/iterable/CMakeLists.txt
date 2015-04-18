# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS at drop drop_until drop_while last)
    Benchmark_add_dataset(dataset.iterable.hana_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: '#include <boost/hana/tuple.hpp>',
                iterable: \"boost::hana::make<boost::hana::Tuple>(#{xs})\",
                input_size: n
            }
        }"
    )

    Benchmark_add_dataset(dataset.iterable.std_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: '
                    #include <boost/hana/ext/std/tuple.hpp>
                    #include <tuple>
                ',
                iterable: \"std::make_tuple(#{xs})\",
                input_size: n
            }
        }"
    )

    Benchmark_add_dataset(dataset.iterable.hana_range.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '#include <boost/hana/range.hpp>',
                iterable: \"::boost::hana::range_c<unsigned long, 0, #{n}>\",
                input_size: n
            }
        }"
    )

    set(_feature_plot_suffix_EXECUTION_TIME "etime")
    set(_feature_plot_suffix_COMPILATION_TIME "ctime")
    set(_feature_plot_suffix_MEMORY_USAGE "mem")
    foreach(_feature IN ITEMS EXECUTION_TIME COMPILATION_TIME MEMORY_USAGE)
        set(_suffix "${_feature_plot_suffix_${_feature}}")
        Benchmark_add_plot(benchmark.iterable.${method}.${_suffix}
            TITLE "${method}"
            FEATURE "${_feature}"
            OUTPUT "${method}.${_suffix}.png"

            CURVE
                TITLE "hana::tuple"
                DATASET dataset.iterable.hana_tuple.${method}

            CURVE
                TITLE "std::tuple"
                DATASET dataset.iterable.std_tuple.${method}

            CURVE
                TITLE "hana::range"
                DATASET dataset.iterable.hana_range.${method}
        )
    endforeach()

    add_custom_target(benchmark.iterable.${method}
        DEPENDS benchmark.iterable.${method}.etime
                benchmark.iterable.${method}.ctime
                benchmark.iterable.${method}.mem)
endforeach()
