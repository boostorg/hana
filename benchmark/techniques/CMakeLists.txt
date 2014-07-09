foreach(i IN ITEMS 1 8 32 64 128 256)
    boost_hana_add_curve_from_source(benchmark.techniques.foldl.linear
        unroll_${i}
        techniques/foldl/linear.cpp
        "(0..1000).step(25).map { |n| { x: n, unroll: ${i} }}"
    )

    boost_hana_add_curve_from_source(benchmark.techniques.foldl.log
        unroll_${i}
        techniques/foldl/log.cpp
        "(0..1000).step(25).map { |n| { x: n, unroll: ${i} }}"
    )
endforeach()
