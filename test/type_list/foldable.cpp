/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/type.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2; struct x3;

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(type_list<>, f),
            f()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(type_list<x0>, f),
            f(type<x0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(type_list<x0, x1>, f),
            f(type<x0>, type<x1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(type_list<x0, x1, x2>, f),
            f(type<x0>, type<x1>, type<x2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unpack(type_list<x0, x1, x2, x3>, f),
            f(type<x0>, type<x1>, type<x2>, type<x3>)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            type_list<>,
            type_list<x0>,
            type_list<x0, x1>,
            type_list<x0, x1, x2>,
            type_list<x0, x1, x2, x3>
        ));
    }
}
