# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

Benchmark_add_plot(benchmark.detail.variadic.reverse_apply
    TITLE "variadic::reverse_apply"
    OUTPUT "reverse_apply.png"
    FEATURE COMPILATION_TIME
    CURVE
        TITLE "flat"
        FILE "variadic/reverse_apply.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '#include <boost/hana/detail/variadic/reverse_apply/flat.hpp>',
                    reverse_apply: 'reverse_apply_flat',
                    input_size: n
                }
            }"

    CURVE
        TITLE "unrolled"
        FILE "variadic/reverse_apply.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '#include <boost/hana/detail/variadic/reverse_apply/unrolled.hpp>',
                    reverse_apply: 'reverse_apply_unrolled',
                    input_size: n
                }
            }"
)