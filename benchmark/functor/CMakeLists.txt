# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)


foreach(method IN ITEMS adjust fill transform replace)
    Benchmark_add_dataset(dataset.functor.hana_tuple.${method}
        FILE "${method}.cpp"
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i|
                \"boost::hana::benchmark::object<#{i}>{}\"
            }.join(', ')
            {
                setup: '#include <boost/hana/tuple.hpp>',
                functor: \"boost::hana::tuple(#{xs})\",
                input_size: n
            }
        }"
    )

    Benchmark_add_dataset(dataset.functor.std_tuple.${method}
        FILE "${method}.cpp"
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            xs = (1..n).to_a.map { |i|
                \"boost::hana::benchmark::object<#{i}>{}\"
            }.join(', ')
            {
                setup: '
                    #include <boost/hana/ext/std/tuple.hpp>
                    #include <tuple>
                ',
                functor: \"std::make_tuple(#{xs})\",
                input_size: n
            }
        }"
    )

    Benchmark_add_plot(benchmark.functor.${method}.etime
        TITLE "${method}"
        FEATURE EXECUTION_TIME
        OUTPUT "${method}.etime.png"

        CURVE
            TITLE "hana::tuple"
            DATASET dataset.functor.hana_tuple.${method}

        CURVE
            TITLE "std::tuple"
            DATASET dataset.functor.std_tuple.${method}
    )

    Benchmark_add_plot(benchmark.functor.${method}.ctime
        TITLE "${method}"
        FEATURE COMPILATION_TIME
        OUTPUT "${method}.ctime.png"

        CURVE
            TITLE "hana::tuple"
            DATASET dataset.functor.hana_tuple.${method}

        CURVE
            TITLE "std::tuple"
            DATASET dataset.functor.std_tuple.${method}
    )

    Benchmark_add_plot(benchmark.functor.${method}.mem
        TITLE "${method}"
        FEATURE MEMORY_USAGE
        OUTPUT "${method}.mem.png"

        CURVE
            TITLE "hana::tuple"
            DATASET dataset.functor.hana_tuple.${method}

        CURVE
            TITLE "std::tuple"
            DATASET dataset.functor.std_tuple.${method}
    )

    add_custom_target(benchmark.functor.${method}
        DEPENDS benchmark.functor.${method}.etime
                benchmark.functor.${method}.ctime
                benchmark.functor.${method}.mem)
endforeach()
