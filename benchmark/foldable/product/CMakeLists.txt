# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Compile-time benchmarks
##############################################################################

# `unsigned long long` generally overflows around factorial(20).
# The product of a range [1, n) is exactly factorial(n-1), so we
# make sure not to overflow.
Benchmark_add_dataset(dataset.foldable.product.compile.hana.range
    FILE "compile/hana.range.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "(1..21).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.product.compile.hana.tuple.heterogeneous
    FILE "compile/hana.tuple.heterogeneous.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.product.compile.hana.tuple.homogeneous
    FILE "compile/hana.tuple.homogeneous.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.product.compile.hana.tuple_c
    FILE "compile/hana.tuple_c.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n| { input_size: n } }"
)

##############################################################################
# Runtime benchmarks
##############################################################################
Benchmark_add_dataset(dataset.foldable.product.execute.hana.tuple.homogeneous
    FILE "execute/hana.tuple.homogeneous.cpp"
    FEATURES EXECUTION_TIME
    ENV "(0..20).map { |n| { input_size: n } }"
)
