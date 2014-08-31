# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS count foldl foldl1 foldr foldr1 for_each length maximum maximum_by minimum minimum_by product sum unpack)
    boost_hana_add_plot_with_name(benchmark.foldable.${method} ${method})

    # Range
    boost_hana_add_curve_from_source(benchmark.foldable.${method} Range ${method}.cpp
        "
        ((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: \"#include <boost/hana/range/foldable.hpp>\",
                foldable: \"boost::hana::range_c<int, 0, #{n}>\",
                x: n
            }
        }
        "
    )

    # Methods requiring `Integral` elements in the `Foldable`.
    if (${method} MATCHES "maximum|minimum|product|sum")
        # Tuple
        boost_hana_add_curve_from_source(benchmark.foldable.${method} Tuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')

                {
                    setup: \"
                        #include <boost/hana/integral.hpp>
                        #include <boost/hana/tuple/foldable.hpp>
                    \",
                    foldable: \"boost::hana::tuple(#{xs})\",
                    x: n
                }
            }
            "
        )

        # StdTuple
        boost_hana_add_curve_from_source(benchmark.foldable.${method} StdTuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')

                {
                    setup: \"
                        #include <boost/hana/ext/std/tuple/foldable.hpp>
                        #include <boost/hana/integral.hpp>
                        #include <tuple>
                    \",
                    foldable: \"std::make_tuple(#{xs})\",
                    x: n
                }
            }
            "
        )
    else()
        # Tuple
        boost_hana_add_curve_from_source(benchmark.foldable.${method} Tuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/tuple/foldable.hpp>
                        template <int i> struct x { };
                    \",
                    foldable: \"boost::hana::tuple(#{xs})\",
                    x: n
                }
            }
            "
        )

        # StdTuple
        boost_hana_add_curve_from_source(benchmark.foldable.${method} StdTuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/ext/std/tuple/foldable.hpp>
                        #include <tuple>
                        template <int i> struct x { };
                    \",
                    foldable: \"std::make_tuple(#{xs})\",
                    x: n
                }
            }
            "
        )
    endif()
endforeach()
