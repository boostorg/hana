/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/tuple_cartesian_product.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;
using detail::tuple_cartesian_product;


static_assert(
    tuple_cartesian_product(make_tuple(1), make_tuple('a', 'b')) == make_tuple(
        make_tuple(1, 'a'),
        make_tuple(1, 'b')
    )
, "");

static_assert(
    tuple_cartesian_product(make_tuple(1, 2), make_tuple('a')) == make_tuple(
        make_tuple(1, 'a'),
        make_tuple(2, 'a')
    )
, "");

static_assert(
    tuple_cartesian_product(make_tuple(1, 2), make_tuple('a', 'b')) == make_tuple(
        make_tuple(1, 'a'),
        make_tuple(1, 'b'),
        make_tuple(2, 'a'),
        make_tuple(2, 'b')
    )
, "");

static_assert(
    tuple_cartesian_product(
        make_tuple(1),
        make_tuple('a'),
        make_tuple(1.f),
        make_tuple(1l, 2l)
    ) == make_tuple(
        make_tuple(1, 'a', 1.f, 1l),
        make_tuple(1, 'a', 1.f, 2l)
    )
, "");

static_assert(
    tuple_cartesian_product(
        make_tuple(1),
        make_tuple('a'),
        make_tuple(1.f),
        make_tuple(1l, 2l),
        make_tuple(nullptr)
    ) == make_tuple(
        make_tuple(1, 'a', 1.f, 1l, nullptr),
        make_tuple(1, 'a', 1.f, 2l, nullptr)
    )
, "");


int main() { }
