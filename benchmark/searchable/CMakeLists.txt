# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS all any elem find lookup none)
    boost_hana_add_plot_with_name(benchmark.searchable.${method} ${method})

    # Tuple
    boost_hana_add_curve_from_source(benchmark.searchable.${method} Tuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/tuple/searchable.hpp>
                    template <int i> struct x { };
                \",
                searchable: \"boost::hana::tuple(#{xs})\",
                x: n
            }
        }
        "
    )

    # StdTuple
    boost_hana_add_curve_from_source(benchmark.searchable.${method} StdTuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/ext/std/tuple/searchable.hpp>
                    #include <tuple>
                    template <int i> struct x { };
                \",
                searchable: \"std::make_tuple(#{xs})\",
                x: n
            }
        }
        "
    )
endforeach()

# `subset` requires `Comparable` elements in the `Searchable`
boost_hana_add_plot_with_name(benchmark.searchable.subset subset)

# Tuple
boost_hana_add_curve_from_source(benchmark.searchable.subset Tuple subset.cpp
    "
    ((1..50).to_a + (51..500).step(25).to_a).map { |n|
        xs = (1..n).to_a.map { |i|
            \"boost::hana::integral<int, #{i}>\"
        }.join(', ')
        {
            setup: \"
                #include <boost/hana/integral/comparable.hpp>
                #include <boost/hana/tuple/searchable.hpp>
            \",
            searchable: \"boost::hana::tuple(#{xs})\",
            x: n
        }
    }
    "
)

# StdTuple
boost_hana_add_curve_from_source(benchmark.searchable.subset StdTuple subset.cpp
    "
    ((1..50).to_a + (51..500).step(25).to_a).map { |n|
        xs = (1..n).to_a.map { |i|
            \"boost::hana::integral<int, #{i}>\"
        }.join(', ')
        {
            setup: \"
                #include <boost/hana/ext/std/tuple/searchable.hpp>
                #include <boost/hana/integral/comparable.hpp>
                #include <tuple>
            \",
            searchable: \"std::make_tuple(#{xs})\",
            x: n
        }
    }
    "
)
