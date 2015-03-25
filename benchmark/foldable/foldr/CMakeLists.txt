# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Compile-time benchmarks
##############################################################################

###################
# Hana
###################

Benchmark_add_dataset(dataset.foldable.foldr.compile.hana.tuple.heterogeneous
    FILE "compile/hana.tuple.heterogeneous.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..90).step(10).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.foldr.compile.hana.tuple.homogeneous
    FILE "compile/hana.tuple.homogeneous.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..90).step(10).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.foldr.compile.hana.tuple_c
    FILE "compile/hana.tuple_c.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..90).step(10).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.foldr.compile.hana.tuple_t
    FILE "compile/hana.tuple_t.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..90).step(10).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.foldr.compile.hana.tuple_t.metafunction
    FILE "compile/hana.tuple_t.metafunction.cpp"
    FEATURES COMPILATION_TIME MEMORY_USAGE
    ENV "((1..50).to_a + (51..90).step(10).to_a).map { |n| { input_size: n } }"
)

###################
# Fusion and MPL
###################
if (Boost_FOUND)
    Benchmark_add_dataset(dataset.foldable.foldr.compile.fusion.vector.heterogeneous
        FILE "compile/fusion.vector.heterogeneous.cpp"
        FEATURES COMPILATION_TIME MEMORY_USAGE
        ENV "(1..20).to_a.map { |n| { input_size: n } }"
    )

    Benchmark_add_dataset(dataset.foldable.foldr.compile.fusion.vector.homogeneous
        FILE "compile/fusion.vector.homogeneous.cpp"
        FEATURES COMPILATION_TIME MEMORY_USAGE
        ENV "(1..20).to_a.map { |n| { input_size: n } }"
    )

    Benchmark_add_dataset(dataset.foldable.foldr.compile.mpl.vector
        FILE "compile/mpl.vector.cpp"
        FEATURES COMPILATION_TIME MEMORY_USAGE
        ENV "(1..20).to_a.map { |n| { input_size: n } }"
    )
endif()


##############################################################################
# Runtime benchmarks
##############################################################################
Benchmark_add_dataset(dataset.foldable.foldr.execute.hana.tuple.homogeneous
    FILE "execute/hana.tuple.homogeneous.cpp"
    FEATURES EXECUTION_TIME
    ENV "(0..20).map { |n| { input_size: n } }"
)

###################
# Fusion
###################
if (Boost_FOUND)
    Benchmark_add_dataset(dataset.foldable.foldr.execute.fusion.vector
        FILE "execute/fusion.vector.cpp"
        FEATURES EXECUTION_TIME
        ENV "(0..20).map { |n| { input_size: n } }"
    )
endif()
