foreach(i IN ITEMS 1 8 32 64 128 255) # 255 instead of 256 because of the recursion limit on Clang
    boost_hana_add_curve_from_source(benchmark.techniques.foldl.linear
        unroll_${i}
        foldl/linear.cpp
        "(0..1000).step(25).map { |n| { x: n, unroll: ${i} }}"
    )

    boost_hana_add_curve_from_source(benchmark.techniques.foldl.log
        unroll_${i}
        foldl/log.cpp
        "(0..1000).step(25).map { |n| { x: n, unroll: ${i} }}"
    )
endforeach()
