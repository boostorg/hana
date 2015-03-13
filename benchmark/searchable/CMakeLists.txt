# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS all_of any_of elem find_if find none_of subset)
    # Methods requiring the contents of the Searchable to be Comparable.
    if (${method} MATCHES "subset")
        set(hana_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral_constant<int, #{i}>\"
                }.join(', ')
                {
                    setup: '
                        #include <boost/hana/integral_constant.hpp>
                        #include <boost/hana/tuple.hpp>
                    ',
                    searchable: \"boost::hana::make<boost::hana::Tuple>(#{xs})\",
                    input_size: n
                }
            }"
        )

        set(std_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral_constant<int, #{i}>\"
                }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple.hpp>
                        #include <boost/hana/integral_constant.hpp>
                        #include <tuple>
                    ',
                    searchable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }"
        )
    else()
        set(hana_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple.hpp>',
                    searchable: \"boost::hana::make<boost::hana::Tuple>(#{xs})\",
                    input_size: n
                }
            }"
        )

        set(std_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple.hpp>
                        #include <tuple>
                    ',
                    searchable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }"
        )
    endif()

    Benchmark_add_dataset(dataset.searchable.hana_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "${hana_tuple_env}"
    )

    Benchmark_add_dataset(dataset.searchable.std_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "${std_tuple_env}"
    )

    set(_feature_plot_suffix_EXECUTION_TIME "etime")
    set(_feature_plot_suffix_COMPILATION_TIME "ctime")
    set(_feature_plot_suffix_MEMORY_USAGE "mem")
    foreach(_feature IN ITEMS EXECUTION_TIME COMPILATION_TIME MEMORY_USAGE)
        set(_suffix "${_feature_plot_suffix_${_feature}}")
        Benchmark_add_plot(benchmark.searchable.${method}.${_suffix}
            TITLE "${method}"
            FEATURE "${_feature}"
            OUTPUT "${method}.${_suffix}.png"

            CURVE
                TITLE "hana::tuple"
                DATASET dataset.searchable.hana_tuple.${method}

            CURVE
                TITLE "std::tuple"
                DATASET dataset.searchable.std_tuple.${method}
        )
    endforeach()

    add_custom_target(benchmark.searchable.${method}
        DEPENDS benchmark.searchable.${method}.etime
                benchmark.searchable.${method}.ctime
                benchmark.searchable.${method}.mem)

endforeach()
