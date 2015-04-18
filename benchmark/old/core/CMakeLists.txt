# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

Benchmark_add_plot(benchmark.core.datatype
    TITLE "Implementations of datatype<>"
    FEATURE COMPILATION_TIME
    CURVE
        TITLE "overload resolution"
        FILE "datatype/overload.cpp"
        ENV "(0...1000).step(5).map { |n| {input_size: n} }"
        COMPILER_FLAGS -std=c++1y -fsyntax-only

    CURVE
        TITLE "specialization"
        FILE "datatype/specialization.cpp"
        ENV "(0...1000).step(5).map { |n| {input_size: n} }"
        COMPILER_FLAGS -std=c++1y -fsyntax-only
)