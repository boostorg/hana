# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

Benchmark_add_plot(benchmark.general.forward
    TITLE "std::forward"
    FEATURE COMPILATION_TIME

    CURVE
        TITLE "with"
        FILE "forward.cpp"
        ENV "(0..10000).step(25).map { |n| { input_size: n, use_forward: true } }"

    CURVE
        TITLE "without"
        FILE "forward.cpp"
        ENV "(0..10000).step(25).map { |n| { input_size: n, use_forward: false } }"
)