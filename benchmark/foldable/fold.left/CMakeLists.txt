# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Compile-time benchmarks
##############################################################################

###################
# Hana
###################

Benchmark_add_dataset(dataset.foldable.fold.left.compile.hana.tuple.heterogeneous
    FILE "compile/hana.tuple.heterogeneous.cpp"
    FEATURES COMPILATION_TIME
    ENV "((1...50).to_a + (50..400).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.fold.left.compile.hana.tuple.homogeneous
    FILE "compile/hana.tuple.homogeneous.cpp"
    FEATURES COMPILATION_TIME
    ENV "((1...50).to_a + (50..400).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.fold.left.compile.hana.tuple_c
    FILE "compile/hana.tuple_c.cpp"
    FEATURES COMPILATION_TIME
    ENV "((1...50).to_a + (50..400).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.fold.left.compile.hana.tuple_t
    FILE "compile/hana.tuple_t.cpp"
    FEATURES COMPILATION_TIME
    ENV "((1...50).to_a + (50..400).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_dataset(dataset.foldable.fold.left.compile.hana.tuple_t.metafunction
    FILE "compile/hana.tuple_t.metafunction.cpp"
    FEATURES COMPILATION_TIME
    ENV "((1...50).to_a + (50..400).step(25).to_a).map { |n| { input_size: n } }"
)

Benchmark_add_plot(plot.foldable.fold.left.compile
    TITLE "Compile-time behavior of left fold"
    FEATURE "COMPILATION_TIME"
    OUTPUT "fold.left.compile.png"

    CURVE   TITLE "Heterogeneous hana::Tuple"
            DATASET dataset.foldable.fold.left.compile.hana.tuple.heterogeneous

    CURVE   TITLE "Homogeneous hana::Tuple"
            DATASET dataset.foldable.fold.left.compile.hana.tuple.homogeneous

    CURVE   TITLE "hana::tuple_c"
            DATASET dataset.foldable.fold.left.compile.hana.tuple_c

    CURVE   TITLE "hana::tuple_t with normal function"
            DATASET dataset.foldable.fold.left.compile.hana.tuple_t

    CURVE   TITLE "hana::tuple_t with hana::Metafunction"
            DATASET dataset.foldable.fold.left.compile.hana.tuple_t.metafunction
)

###################
# Fusion and MPL
###################
if (Boost_FOUND)
    Benchmark_add_dataset(dataset.foldable.fold.left.compile.fusion.vector.heterogeneous
        FILE "compile/fusion.vector.heterogeneous.cpp"
        FEATURES COMPILATION_TIME
        ENV "(1..50).to_a.map { |n| { input_size: n } }"
    )

    Benchmark_add_dataset(dataset.foldable.fold.left.compile.fusion.vector.homogeneous
        FILE "compile/fusion.vector.homogeneous.cpp"
        FEATURES COMPILATION_TIME
        ENV "(1..50).to_a.map { |n| { input_size: n } }"
    )

    Benchmark_add_dataset(dataset.foldable.fold.left.compile.mpl.vector
        FILE "compile/mpl.vector.cpp"
        FEATURES COMPILATION_TIME
        ENV "(1..20).to_a.map { |n| { input_size: n } }"
    )

    Benchmark_add_curves(PLOT plot.foldable.fold.left.compile
        CURVE   TITLE "Heterogeneous fusion::vector"
                DATASET dataset.foldable.fold.left.compile.fusion.vector.heterogeneous

        CURVE   TITLE "Homogeneous fusion::vector"
                DATASET dataset.foldable.fold.left.compile.fusion.vector.homogeneous

        CURVE   TITLE "mpl::vector with a MPL metafunction class"
                DATASET dataset.foldable.fold.left.compile.mpl.vector
    )
endif()


##############################################################################
# Runtime benchmarks
##############################################################################
Benchmark_add_dataset(dataset.foldable.fold.left.execute.hana.tuple
    FILE "execute/hana.tuple.cpp"
    FEATURES EXECUTION_TIME
    ENV "(0..50).map { |n| { input_size: n } }"
)

Benchmark_add_plot(plot.foldable.fold.left.execute
    TITLE "Runtime behavior of left fold"
    FEATURE "EXECUTION_TIME"
    OUTPUT "fold.left.execute.png"

    CURVE   TITLE "Homogeneous hana::Tuple"
            DATASET dataset.foldable.fold.left.execute.hana.tuple
)

###################
# Fusion
###################
if (Boost_FOUND)
    Benchmark_add_dataset(dataset.foldable.fold.left.execute.fusion.vector
        FILE "execute/fusion.vector.cpp"
        FEATURES EXECUTION_TIME
        ENV "(0..50).map { |n| { input_size: n } }"
    )

    Benchmark_add_curve(PLOT plot.foldable.fold.left.execute
        TITLE "Homogeneous fusion::vector"
        DATASET dataset.foldable.fold.left.execute.fusion.vector
    )
endif()
