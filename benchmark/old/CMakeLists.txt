# In this folder, the benchmarks are organized as follow:
#
# instance/ -- All isomorphic instances of List
# typeclass/method.cpp -- A benchmark for a method of a type class instantiated by
#                      -- List, written to work on any instance of List (it's possible
#                      -- because they are all isomorphic)
# method.cpp  -- A benchmark for a method of the List type class
#
#
# todo:
# How to benchmark stuff like TypeList, which is only some kind of
# restricted List?


file(GLOB_RECURSE BOOST_HANA_LIST_INSTANCES
     RELATIVE "${Boost.Hana_SOURCE_DIR}/benchmark"
     "instance/*")

if (NOT Boost_FOUND)
    message(STATUS
    "The Boost library headers were not found; benchmarks for Boost-adapted "
    "List instances (benchmark/list/instance/ext/boost/*) won't be available.")
    file(GLOB_RECURSE BOOST_HANA_LIST_INSTANCES_DEPENDING_ON_BOOST
         RELATIVE "${Boost.Hana_SOURCE_DIR}/benchmark"
         "instance/ext/boost/*")
    list(REMOVE_ITEM BOOST_HANA_LIST_INSTANCES ${BOOST_HANA_LIST_INSTANCES_DEPENDING_ON_BOOST})
endif()

# Add a plot for all the (isomorphic) instances of List.
function(boost_hana_add_list_plot plot_name cpp_file envs)
    boost_hana_add_plot(${plot_name})
    foreach(instance_file IN LISTS BOOST_HANA_LIST_INSTANCES)
        string(CONFIGURE ${envs} envs_for_this_instance @ONLY)

        string(REPLACE "list/instance/" "" instance_name ${instance_file})
        string(REPLACE ".hpp" "" instance_name ${instance_name})
        string(REPLACE "/" "." instance_name ${instance_name})

        boost_hana_add_curve_from_source(${plot_name}
            ${instance_name}
            ${cpp_file}
            ${envs_for_this_instance}
        )
    endforeach()
endfunction()

# List
boost_hana_add_list_plot(
    benchmark.list.partition.filter
    partition/filter.cpp
    "((0..50).to_a + (51..500).step(25).to_a).map { |n| { x: n, instance:\"@instance_file@\" } }"
)

boost_hana_add_list_plot(
    benchmark.list.partition.foldl
    partition/foldl.cpp
    "((0..50).to_a + (51..500).step(25).to_a).map { |n| { x: n, instance:\"@instance_file@\" } }"
)
