# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS count foldl foldl1 foldr foldr1 for_each length maximum maximum_by minimum minimum_by product sum unpack)
    Benchmark_add_plot(benchmark.foldable.${method}
        TITLE "${method}"
        FEATURE COMPILATION_TIME
        OUTPUT "${method}.png"

        CURVE
            TITLE "Range"
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '#include <boost/hana/range/foldable.hpp>',
                    foldable: \"boost::hana::range_c<int, 0, #{n}>\",
                    input_size: n
                }
            }"
    )

    # Methods requiring `Integral` elements in the `Foldable`.
    if(${method} MATCHES "maximum|minimum|product|sum")
        set(tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')

                {
                    setup: '
                        #include <boost/hana/integral.hpp>
                        #include <boost/hana/tuple/foldable.hpp>
                    ',
                    foldable: \"boost::hana::tuple(#{xs})\",
                    input_size: n
                }
            }
        ")

        set(std_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')

                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/foldable.hpp>
                        #include <boost/hana/integral.hpp>
                        #include <tuple>
                    ',
                    foldable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
        ")
    else()
        set(tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple/foldable.hpp>',
                    foldable: \"boost::hana::tuple(#{xs})\",
                    input_size: n
                }
            }
        ")

        set(std_tuple_env "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/foldable.hpp>
                        #include <tuple>
                    ',
                    foldable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
        ")
    endif()

    Benchmark_add_curves(
        PLOT benchmark.foldable.${method}
        CURVE
            TITLE "Tuple"
            FILE "${method}.cpp"
            ENV "${tuple_env}"

        CURVE
            TITLE "StdTuple"
            FILE "${method}.cpp"
            ENV "${std_tuple_env}"
    )
endforeach()
