# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS adjust fill fmap replace)
    boost_hana_add_plot_with_name(benchmark.functor.${method} ${method})

    # Tuple
    boost_hana_add_curve_from_source(benchmark.functor.${method} Tuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/tuple/functor.hpp>
                    template <int i> struct x { };
                \",
                functor: \"boost::hana::tuple(#{xs})\",
                x: n
            }
        }
        "
    )

    # StdTuple
    boost_hana_add_curve_from_source(benchmark.functor.${method} StdTuple ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
            {
                setup: \"
                    #include <boost/hana/ext/std/tuple/functor.hpp>
                    #include <tuple>
                    template <int i> struct x { };
                \",
                functor: \"std::make_tuple(#{xs})\",
                x: n
            }
        }
        "
    )
endforeach()
