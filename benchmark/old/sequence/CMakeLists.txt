# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

function(capitalize word output)
    string(SUBSTRING ${word} 0 1 _first_letter)
    string(TOUPPER ${_first_letter} _first_letter)
    string(REGEX REPLACE "^.(.*)" "${_first_letter}\\1" _output "${word}")
    set(${output} "${_output}" PARENT_SCOPE)
endfunction()

foreach(method IN ITEMS filter group_by intersperse
                        make permutations remove_at reverse
                        sort take take_while zip_with)

    Benchmark_add_dataset(dataset.sequence.hana_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '#include <boost/hana/tuple.hpp>',
                datatype: 'boost::hana::Tuple',
                input_size: n
            }
        }
        "
    )

    Benchmark_add_dataset(dataset.sequence.std_tuple.${method}
        FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
        FILE "${method}.cpp"
        ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
            {
                setup: '
                    #include <boost/hana/ext/std/tuple.hpp>
                    #include <tuple>
                ',
                datatype: 'boost::hana::ext::std::Tuple',
                input_size: n
            }
        }
        "
    )

    if(Boost_FOUND)
        Benchmark_add_dataset(dataset.sequence.boost_tuple.${method}
            FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '
                        #include <boost/hana/ext/boost/tuple.hpp>
                        #include <boost/tuple/tuple.hpp>
                    ',
                    datatype: 'boost::hana::ext::boost::Tuple',
                    input_size: n
                }
            }
            "
            ADDITIONAL_COMPILER_FLAGS "-I${Boost_INCLUDE_DIRS}"
        )

        Benchmark_add_dataset(dataset.sequence.boost_fusion_tuple.${method}
            FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
            FILE "${method}.cpp"
            ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                {
                    setup: '
                        #include <boost/hana/ext/boost/fusion/tuple.hpp>
                        #include <boost/fusion/tuple.hpp>
                    ',
                    datatype: 'boost::hana::ext::boost::fusion::Tuple',
                    input_size: n
                }
            }
            "
            ADDITIONAL_COMPILER_FLAGS "-I${Boost_INCLUDE_DIRS}"
        )

        foreach(sequence IN ITEMS vector list deque)
            capitalize(${sequence} Sequence)
            Benchmark_add_dataset(dataset.sequence.boost_fusion_${sequence}.${method}
                FEATURES COMPILATION_TIME EXECUTION_TIME MEMORY_USAGE
                FILE "${method}.cpp"
                ENV "((1..50).to_a + (51..500).step(25).to_a).map { |n|
                    {
                        setup: '
                            #include <boost/hana/ext/boost/fusion/${sequence}.hpp>
                            #include <boost/fusion/container/${sequence}/${sequence}.hpp>
                        ',
                        datatype: 'boost::hana::ext::boost::fusion::${Sequence}',
                        input_size: n
                    }
                }
                "
                ADDITIONAL_COMPILER_FLAGS "-I${Boost_INCLUDE_DIRS}"
            )
        endforeach()
    endif()

    set(_feature_plot_suffix_EXECUTION_TIME "etime")
    set(_feature_plot_suffix_COMPILATION_TIME "ctime")
    set(_feature_plot_suffix_MEMORY_USAGE "mem")
    foreach(_feature IN ITEMS EXECUTION_TIME COMPILATION_TIME MEMORY_USAGE)
        set(_suffix "${_feature_plot_suffix_${_feature}}")
        Benchmark_add_plot(benchmark.sequence.${method}.${_suffix}
            TITLE "${method}"
            FEATURE "${_feature}"
            OUTPUT "${method}.${_suffix}.png"

            CURVE
                TITLE "hana::tuple"
                DATASET dataset.sequence.hana_tuple.${method}

            CURVE
                TITLE "std::tuple"
                DATASET dataset.sequence.std_tuple.${method}
        )

        if(Boost_FOUND)
            Benchmark_add_curves(PLOT benchmark.sequence.${method}.${_suffix}
                CURVE
                    TITLE "boost::tuple"
                    DATASET dataset.sequence.boost_tuple.${method}
                CURVE
                    TITLE "boost::fusion::tuple"
                    DATASET dataset.sequence.boost_fusion_tuple.${method}
                CURVE
                    TITLE "boost::fusion::vector"
                    DATASET dataset.sequence.boost_fusion_vector.${method}
                CURVE
                    TITLE "boost::fusion::list"
                    DATASET dataset.sequence.boost_fusion_list.${method}
                CURVE
                    TITLE "boost::fusion::deque"
                    DATASET dataset.sequence.boost_fusion_deque.${method}
            )
        endif()
    endforeach()

    add_custom_target(benchmark.sequence.${method}
        DEPENDS benchmark.sequence.${method}.etime
                benchmark.sequence.${method}.ctime
                benchmark.sequence.${method}.mem)
endforeach()
