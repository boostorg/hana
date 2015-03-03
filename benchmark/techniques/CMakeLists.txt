# Copyright Louis Dionne 2015
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

foreach(operation IN ITEMS get make)
    Benchmark_add_plot(benchmark.techniques.closure.${operation}
        TITLE "Closure implementations"
        FEATURE COMPILATION_TIME
        CURVE
            TITLE "linked list of smaller tuples"
            FILE "closure/${operation}/linked_list.cpp"
            ENV "((1..50).to_a + (51..1000).step(25).to_a).map { |n|
                {input_size: n}
            }"
            COMPILATION_TIMEOUT 5

        CURVE
            TITLE "multiple inheritance"
            FILE "closure/${operation}/multiple.cpp"
            ENV "((1..50).to_a + (51..1000).step(25).to_a).map { |n|
                {input_size: n}
            }"
            COMPILATION_TIMEOUT 5

        CURVE
            TITLE "lambda based"
            FILE "closure/${operation}/lambda.cpp"
            ENV "((1..50).to_a + (51..1000).step(25).to_a).map { |n|
                {input_size: n}
            }"
            COMPILATION_TIMEOUT 5
    )
endforeach()


Benchmark_add_plot(benchmark.techniques.at
    TITLE "Random access in a parameter pack"
    FEATURE COMPILATION_TIME
    CURVE
        TITLE "arg<n>"
        FILE "at_index/arg.cpp"
        ENV "(1..1000).step(25).map { |n| { input_size: n }}"

    CURVE
        TITLE "variadic::at"
        FILE "at_index/variadic.cpp"
        ENV "(1..1000).step(25).map { |n| { input_size: n }}"
)