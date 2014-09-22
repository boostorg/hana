# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS at drop drop_until drop_while last)
    Benchmark_add_plot(benchmark.iterable.${method}
        TITLE "${method}"
        OUTPUT "${method}.png"
        FEATURE COMPILATION_TIME
        CURVE
            TITLE "hana::tuple"
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple/iterable.hpp>',
                    iterable: \"boost::hana::tuple(#{xs})\",
                    input_size: n
                }
            }"

        CURVE
            TITLE "std::tuple"
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '
                        #include <boost/hana/ext/std/tuple/iterable.hpp>
                        #include <tuple>
                    ',
                    iterable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }"
    )
endforeach()
