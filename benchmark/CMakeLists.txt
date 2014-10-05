# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

include(Benchmarks)

if(TARGET BENCHMARK_ALL_PLOTS)
    add_custom_target(benchmarks DEPENDS BENCHMARK_ALL_PLOTS)
endif()

add_subdirectory(core)
add_subdirectory(detail)
add_subdirectory(foldable)
add_subdirectory(functor)
add_subdirectory(general)
add_subdirectory(iterable)
add_subdirectory(list)
add_subdirectory(searchable)
add_subdirectory(techniques)
add_subdirectory(vs)
