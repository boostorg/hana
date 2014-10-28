/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2; struct x3;

namespace boost { namespace hana { namespace test {
    template <>
    auto objects<TypeList> = tuple(
        type_list<>,
        type_list<x0>,
        type_list<x0, x1>,
        type_list<x0, x1, x2>,
        type_list<x0, x1, x2, x3>
    );

    template <>
    auto instances<TypeList> = tuple(
        type<Comparable>,
        type<Foldable>,
        //! @todo TypeList does not satisfy the laws for List, since it can
        //! only hold Types.
#if 0
        type<List>,
#endif
        type<Iterable>
    );
}}}


int main() {
    test::check_datatype<TypeList>();

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(type_list<>, type_list<>));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type_list<x0>, type_list<>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type_list<>, type_list<x0>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(type_list<x0>, type_list<x0>));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type_list<x0, x1>, type_list<x0>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type_list<x0>, type_list<x0, x1>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(type_list<x0, x1>, type_list<x0, x1>));
            BOOST_HANA_CONSTANT_ASSERT(equal(type_list<x0, x1, x2>, type_list<x0, x1, x2>));
        }
    }

    // Iterable
    {
        // head
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0>), type<x0>));
            BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0, x1>), type<x0>));
            BOOST_HANA_CONSTANT_ASSERT(equal(head(type_list<x0, x1, x2>), type<x0>));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_ASSERT(is_empty(type_list<>));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(type_list<x0>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(type_list<x0, x1>)));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0>), type_list<>));
            BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0, x1>), type_list<x1>));
            BOOST_HANA_CONSTANT_ASSERT(equal(tail(type_list<x0, x1, x2>), type_list<x1, x2>));
        }
    }

    // Foldable
    {
        auto f = test::injection([]{});

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
    }

    // List
    {
        // cons
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(type<x0>, type_list<>),
                type_list<x0>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(type<x0>, type_list<x1>),
                type_list<x0, x1>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(type<x0>, type_list<x1, x2>),
                type_list<x0, x1, x2>
            ));
        }

        // nil
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(nil<TypeList>, type_list<>));
        }
    }
}
