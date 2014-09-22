# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

Benchmark_add_plot(benchmark.techniques.foldl.linear
    TITLE "Variadic left folds with linear-depth unrolling"
    FEATURE COMPILATION_TIME
)

Benchmark_add_plot(benchmark.techniques.foldl.log
    TITLE "Variadic left folds with log-depth unrolling"
    FEATURE COMPILATION_TIME
)

# 255 instead of 256 because of the recursion limit on Clang
foreach(i IN ITEMS 1 8 32 64 128 255)
    Benchmark_add_curve(
        PLOT benchmark.techniques.foldl.linear
        TITLE "${i}-depth"
        FILE "foldl/linear.cpp"
        ENV "(0..1000).step(25).map { |n| { input_size: n, unroll: ${i} }}"
    )

    Benchmark_add_curve(
        PLOT benchmark.techniques.foldl.log
        TITLE "${i}-depth"
        FILE "foldl/log.cpp"
        ENV "(0..1000).step(25).map { |n| { input_size: n, unroll: ${i} }}"
    )
endforeach()