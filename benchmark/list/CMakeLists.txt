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
# - Benchcc should allow removing the name of the file beside the progress
#   bar since it's useless in our use case.
# - How to benchmark stuff like TypeList, which is only some kind of
#   restricted List?
# - Create fusion::cons, fusion::list and fusion::vector correctly/more
#   efficiently.


file(GLOB_RECURSE BOOST_HANA_LIST_INSTANCES
     RELATIVE "${Boost.Hana_SOURCE_DIR}/benchmark"
     "instance/*")

# Add a plot for all the isomorphic instances of List.
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

boost_hana_add_list_plot(
    benchmark.list.permutations
    permutations.cpp
    "(0..2).map { |n| { x: n, instance:\"@instance_file@\" } }"
)

boost_hana_add_list_plot(
    benchmark.list.functor.fmap
    functor/fmap.cpp
    "(0..1000).step(25).map { |n| { x: n, instance:\"@instance_file@\" } }"
)

boost_hana_add_list_plot(
    benchmark.list.foldable.foldl
    foldable/foldl.cpp
    "(0..1000).step(25).map { |n| { x: n, instance:\"@instance_file@\" } }"
)

boost_hana_add_list_plot(
    benchmark.list.partition.filter
    partition/filter.cpp
    "(0..100).step(5).map { |n| { x: n, instance:\"@instance_file@\" } }"
)

boost_hana_add_list_plot(
    benchmark.list.partition.foldl
    partition/foldl.cpp
    "(0..100).step(5).map { |n| { x: n, instance:\"@instance_file@\" } }"
)
