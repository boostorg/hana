# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS filter group_by intersperse
                        make permutations remove_at reverse
                        scanl scanl1 scanr scanr1 sort
                        take take_until take_while zip_with)

    Benchmark_add_dataset(dataset.list.hana_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '#include <boost/hana/tuple.hpp>',
                datatype: 'boost::hana::Tuple',
                input_size: n
            }
        }
        "
    )

    Benchmark_add_dataset(dataset.list.std_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '
                    #include <boost/hana/ext/std/tuple.hpp>
                    #include <tuple>
                ',
                datatype: 'boost::hana::ext::std::Tuple',
                input_size: n
            }
        }
        "
    )

    set(_feature_plot_suffix_EXECUTION_TIME "etime")
    set(_feature_plot_suffix_COMPILATION_TIME "ctime")
    set(_feature_plot_suffix_MEMORY_USAGE "mem")
    foreach(_feature IN ITEMS EXECUTION_TIME COMPILATION_TIME MEMORY_USAGE)
        set(_suffix "${_feature_plot_suffix_${_feature}}")
        Benchmark_add_plot(benchmark.list.${method}.${_suffix}
            TITLE "${method}"
            FEATURE "${_feature}"
            OUTPUT "${method}.${_suffix}.png"

            CURVE
                TITLE "hana::tuple"
                DATASET dataset.list.hana_tuple.${method}

            CURVE
                TITLE "std::tuple"
                DATASET dataset.list.std_tuple.${method}
        )
    endforeach()

    add_custom_target(benchmark.list.${method}
        DEPENDS benchmark.list.${method}.etime
                benchmark.list.${method}.ctime
                benchmark.list.${method}.mem)
endforeach()
