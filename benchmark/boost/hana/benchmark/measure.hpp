/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BENCHMARK_MEASURE_HPP
#define BOOST_HANA_BENCHMARK_MEASURE_HPP

#include <boost/hana/detail/constexpr.hpp>

#include <chrono>


namespace boost { namespace hana { namespace benchmark {
    BOOST_HANA_CONSTEXPR_LAMBDA auto measure = [](auto f) {
        auto start = std::chrono::steady_clock::now();
        f();
        auto stop = std::chrono::steady_clock::now();

        return stop - start;
    };
}}}

#endif // !BOOST_HANA_BENCHMARK_MEASURE_HPP
