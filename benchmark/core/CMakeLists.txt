boost_hana_add_curve_from_source(benchmark.core.datatype
    overload
    datatype/overload.cpp
    "(0...1000).step(5).map { |n| {x: n} }"
)

boost_hana_add_curve_from_source(benchmark.core.datatype
    specialization
    datatype/specialization.cpp
    "(0...1000).step(5).map { |n| {x: n} }"
)
