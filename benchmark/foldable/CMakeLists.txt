# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS count foldl foldl1 foldr foldr1 for_each length maximum maximum_by minimum minimum_by product sum unpack)
    # Methods requiring integral `Constant`s (or stronger) in the `Foldable`.
    if(${method} MATCHES "maximum|minimum|product|sum")
        set(hana_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral_constant<int, #{i}>\"
                }.join(', ')

                {
                    setup: '
                        #include <boost/hana/integral_constant.hpp>
                        #include <boost/hana/tuple.hpp>
                    ',
                    foldable: \"boost::hana::make<boost::hana::Tuple>(#{xs})\",
                    input_size: n
                }
            }
        ")

        set(std_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral_constant<int, #{i}>\"
                }.join(', ')

                {
                    setup: '
                        #include <boost/hana/ext/std/tuple.hpp>
                        #include <boost/hana/integral_constant.hpp>
                        #include <tuple>
                    ',
                    foldable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
        ")
    else()
        set(hana_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple.hpp>',
                    foldable: \"boost::hana::make<boost::hana::Tuple>(#{xs})\",
                    input_size: n
                }
            }
        ")

        set(std_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple.hpp>
                        #include <tuple>
                    ',
                    foldable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
        ")
    endif()

    Benchmark_add_dataset(dataset.foldable.range.${method}
        FILE "${method}.cpp"
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '#include <boost/hana/range.hpp>',
                foldable: \"boost::hana::range_c<int, 0, #{n}>\",
                input_size: n
            }
        }"
    )

    Benchmark_add_dataset(dataset.foldable.hana_tuple.${method}
        FILE "${method}.cpp"
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        ENV "${hana_tuple_env}"
    )

    Benchmark_add_dataset(dataset.foldable.std_tuple.${method}
        FILE "${method}.cpp"
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        ENV "${std_tuple_env}"
    )

    set(_feature_plot_suffix_EXECUTION_TIME "etime")
    set(_feature_plot_suffix_COMPILATION_TIME "ctime")
    set(_feature_plot_suffix_MEMORY_USAGE "mem")
    foreach(_feature IN ITEMS EXECUTION_TIME COMPILATION_TIME MEMORY_USAGE)
        set(_suffix "${_feature_plot_suffix_${_feature}}")
        Benchmark_add_plot(benchmark.foldable.${method}.${_suffix}
            TITLE "${method}"
            FEATURE "${_feature}"
            OUTPUT "${method}.${_suffix}.png"

            CURVE
                TITLE "hana::tuple"
                DATASET dataset.foldable.hana_tuple.${method}

            CURVE
                TITLE "std::tuple"
                DATASET dataset.foldable.std_tuple.${method}

            CURVE
                TITLE "hana::range"
                DATASET dataset.foldable.range.${method}
        )
    endforeach()

    add_custom_target(benchmark.foldable.${method}
        DEPENDS benchmark.foldable.${method}.etime
                benchmark.foldable.${method}.ctime
                benchmark.foldable.${method}.mem)

endforeach()
