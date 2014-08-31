# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS at drop drop_until drop_while last)
    boost_hana_add_plot_with_name(benchmark.iterable.${method} ${method})

    # Tuple
    boost_hana_add_curve_from_source(benchmark.iterable.${method} Tuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/tuple/iterable.hpp>
                    template <int i> struct x { };
                \",
                iterable: \"boost::hana::tuple(#{xs})\",
                n: n, x: n
            }
        }
        "
    )

    # StdTuple
    boost_hana_add_curve_from_source(benchmark.iterable.${method} StdTuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/ext/std/tuple/iterable.hpp>
                    #include <tuple>
                    template <int i> struct x { };
                \",
                iterable: \"std::make_tuple(#{xs})\",
                n: n, x: n
            }
        }
        "
    )
endforeach()
