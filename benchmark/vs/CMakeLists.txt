# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

set(_env "
    (1..20).to_a.map { |n|
        types = [
            'long',   'float',       'short',  'double',      'float',
            'long',   'long double', 'long',   'float',       'short',
            'double', 'float',       'long',   'long double', 'long',
            'float',  'short',       'double', 'float',       'long'
        ]
        {
            input_size: n,
            types: types[0...n]
        }
    }
")

Benchmark_add_plot(benchmark.vs.foldl
    TITLE "left fold"
    FEATURE EXECUTION_TIME
    CURVE
        TITLE "hana"
        FILE "foldl/hana.cpp"
        ENV "${_env}"
        ADDITIONAL_COMPILER_FLAGS -O3
)

if(Boost_FOUND)
    Benchmark_add_curve(
        PLOT benchmark.vs.foldl
        TITLE "fusion"
        FILE "foldl/fusion.cpp"
        ENV "${_env}"
        ADDITIONAL_COMPILER_FLAGS -O3 -I"${Boost_INCLUDE_DIRS}"
    )
endif()