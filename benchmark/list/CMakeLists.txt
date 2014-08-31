# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

foreach(method IN ITEMS filter make permutations reverse scanl scanl1 scanr scanr1 sort take take_until take_while zip_with)
    boost_hana_add_plot_with_name(benchmark.list.${method} ${method})

    # Methods requiring the contents of the List to be Integrals.
    if (${method} MATCHES "sort")
        # Tuple
        boost_hana_add_curve_from_source(benchmark.list.${method} Tuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/integral.hpp>
                        #include <boost/hana/tuple/list.hpp>
                    \",
                    list: \"boost::hana::tuple(#{xs})\",
                    n: n, x: n
                }
            }
            "
        )

        # StdTuple
        boost_hana_add_curve_from_source(benchmark.list.${method} StdTuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i|
                    \"boost::hana::integral<int, #{i}>\"
                }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <boost/hana/integral.hpp>
                        #include <tuple>
                    \",
                    list: \"std::make_tuple(#{xs})\",
                    n: n, x: n
                }
            }
            "
        )
    elseif(${method} MATCHES "make")
        # Tuple
        boost_hana_add_curve_from_source(benchmark.list.${method} Tuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: \"
                        #include <boost/hana/tuple/list.hpp>
                    \",
                    datatype: \"boost::hana::Tuple\",
                    n: n, x: n
                }
            }
            "
        )

        # StdTuple
        boost_hana_add_curve_from_source(benchmark.list.${method} StdTuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: \"
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <tuple>
                    \",
                    datatype: \"boost::hana::StdTuple\",
                    n: n, x: n
                }
            }
            "
        )
    else()
        # Tuple
        boost_hana_add_curve_from_source(benchmark.list.${method} Tuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/tuple/list.hpp>
                        template <int i> struct x { };
                    \",
                    list: \"boost::hana::tuple(#{xs})\",
                    n: n, x: n
                }
            }
            "
        )

        # StdTuple
        boost_hana_add_curve_from_source(benchmark.list.${method} StdTuple ${method}.cpp
            "
            ((1..50).to_a + (51..500).step(25).to_a).map { |n|
                xs = (1..n).to_a.map { |i| \"x<#{i}>{}\" }.join(', ')
                {
                    setup: \"
                        #include <boost/hana/ext/std/tuple/list.hpp>
                        #include <tuple>
                        template <int i> struct x { };
                    \",
                    list: \"std::make_tuple(#{xs})\",
                    n: n, x: n
                }
            }
            "
        )
    endif()
endforeach()
