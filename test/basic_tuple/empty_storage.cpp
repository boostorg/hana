// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/basic_tuple.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct empty1 { };
struct empty2 { };
struct empty3 { };


// Make sure the storage of a basic_tuple is compressed.
static_assert(sizeof(hana::basic_tuple<empty1, int>) == sizeof(int), "");
static_assert(sizeof(hana::basic_tuple<int, empty1>) == sizeof(int), "");

// Also make sure that a basic_tuple with only empty members is empty. This is
// important to ensure, for example, that a tuple of tuples of empty objects
// will get the EBO. We also test the nested case to be sure.
static_assert(std::is_empty<hana::basic_tuple<empty1, empty2>>{}, "");
static_assert(std::is_empty<hana::basic_tuple<empty1, hana::basic_tuple<empty2, empty3>>>{}, "");


int main() { }
