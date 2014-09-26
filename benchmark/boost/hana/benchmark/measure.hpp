/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BENCHMARK_MEASURE_HPP
#define BOOST_HANA_BENCHMARK_MEASURE_HPP

#include <boost/hana/detail/constexpr.hpp>

#include <chrono>
#include <iostream>
#include <thread>


namespace boost { namespace hana { namespace benchmark {
    template <int i>
    struct object {
        constexpr object() = default;

        object(object const&) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto measure = [](auto f) {
        constexpr auto repetitions = 1000ull;
        auto start = std::chrono::steady_clock::now();
        for (auto i = repetitions; i > 0; --i) {
            f();
        }
        auto stop = std::chrono::steady_clock::now();

        auto time = std::chrono::duration_cast<std::chrono::microseconds>(
            (stop - start) / repetitions
        );
        std::cout << time.count();
    };
}}}

#endif // !BOOST_HANA_BENCHMARK_MEASURE_HPP
