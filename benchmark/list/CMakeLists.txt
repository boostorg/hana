# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS filter make permutations reverse scanl scanl1 scanr scanr1 sort take take_until take_while zip_with)
    # Methods requiring the contents of the List to be Integrals.
    if (${method} MATCHES "sort")
        # Tuple
        set(hana_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')
                {
                    setup: '
                        #include <boost/hana/integral.hpp>
                        #include <boost/hana/tuple/list.hpp>
                    ',
                    list: \"boost::hana::tuple(#{xs})\",
                    input_size: n
                }
            }
            "
        )

        # StdTuple
        set(std_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <boost/hana/integral.hpp>
                        #include <tuple>
                    ',
                    list: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
            "
        )
    elseif(${method} MATCHES "make")
        # Tuple
        set(hana_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '#include <boost/hana/tuple/list.hpp>',
                    datatype: 'boost::hana::Tuple',
                    input_size: n
                }
            }
            "
        )

        # StdTuple
        set(std_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <tuple>
                    ',
                    datatype: 'boost::hana::StdTuple',
                    input_size: n
                }
            }
            "
        )
    else()
        # Tuple
        set(hana_tuple_env
            "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple/list.hpp>',
                    list: \"boost::hana::tuple(#{xs})\",
                    input_size: n
                }
            }
            "
        )

        # StdTuple
        set(std_tuple_env
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <tuple>
                    ',
                    list: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
            "
        )
    endif()

    Benchmark_add_dataset(dataset.list.hana_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "${hana_tuple_env}"
    )

    Benchmark_add_dataset(dataset.list.std_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "${std_tuple_env}"
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
