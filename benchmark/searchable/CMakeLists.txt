# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS all any elem find lookup none)
    Benchmark_add_plot(benchmark.searchable.${method}
        TITLE "${method}"
        OUTPUT "${method}.png"
        FEATURE COMPILATION_TIME
        CURVE
            TITLE "hana::tuple"
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: '#include <boost/hana/tuple/searchable.hpp>',
                    searchable: \"boost::hana::tuple(#{xs})\",
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
                        #include <boost/hana/ext/std/tuple/searchable.hpp>
                        #include <tuple>
                    ',
                    searchable: \"std::make_tuple(#{xs})\",
                    input_size: n
                }
            }
        "
    )
endforeach()

# `subset` requires `Comparable` elements in the `Searchable`
Benchmark_add_plot(benchmark.searchable.subset
    TITLE "subset"
    OUTPUT "subset.png"
    FEATURE COMPILATION_TIME
    CURVE
        TITLE "hana::tuple"
        FILE "subset.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i|
                \"boost::hana::integral<int, #{i}>\"
            }.join(', ')
            {
                setup: '
                    #include <boost/hana/integral/comparable.hpp>
                    #include <boost/hana/tuple/searchable.hpp>
                ',
                searchable: \"boost::hana::tuple(#{xs})\",
                input_size: n
            }
        }"

    CURVE
        TITLE "std::tuple"
        FILE "subset.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i|
                \"boost::hana::integral<int, #{i}>\"
            }.join(', ')
            {
                setup: '
                    #include <boost/hana/ext/std/tuple/searchable.hpp>
                    #include <boost/hana/integral/comparable.hpp>
                    #include <tuple>
                ',
                searchable: \"std::make_tuple(#{xs})\",
                input_size: n
            }
        }"
)
