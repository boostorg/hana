/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integer_list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>

// instances
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<IntegerList> = tuple(
        //! @todo IntegerList is not actually a List, since it can only hold
        //! Integrals.
#if 0
        type<List>,
#endif
        type<Iterable>
    );

    template <>
    auto objects<IntegerList> = tuple(
        integer_list<int>,
        integer_list<int, 0>,
        integer_list<int, 0, 1>,
        integer_list<int, 0, 1, 2>
    );
}}}


int main() {
    test::check_datatype<IntegerList>();

    // Iterable
    {
        // head
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(head(integer_list<int, 0>), int_<0>));
            BOOST_HANA_CONSTANT_ASSERT(equal(head(integer_list<int, 0, 1>), int_<0>));
            BOOST_HANA_CONSTANT_ASSERT(equal(head(integer_list<int, 0, 1, 2>), int_<0>));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_ASSERT(is_empty(integer_list<int>));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(integer_list<int, 0>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(integer_list<int, 0, 1>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(integer_list<int, 0, 1, 2>)));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                tail(integer_list<int, 0>),
                integer_list<int>
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                tail(integer_list<int, 0, 1>),
                integer_list<int, 1>
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                tail(integer_list<int, 0, 1, 2>),
                integer_list<int, 1, 2>
            ));
        }
    }

    // List
    {
        // cons
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(integral<int, 0>, integer_list<long>),
                integer_list<long, 0>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(integral<int, 0>, integer_list<unsigned int, 1>),
                integer_list<unsigned int, 0, 1>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(integral<long, 0>, integer_list<long long, 1, 2>),
                integer_list<long long, 0, 1, 2>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                cons(integral<unsigned long, 0>, integer_list<unsigned long, 1, 2, 3>),
                integer_list<unsigned long, 0, 1, 2, 3>
            ));
        }

        // nil
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                nil<IntegerList>,
                integer_list<int>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                nil<IntegerList>,
                integer_list<long>
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                nil<IntegerList>,
                integer_list<void>
            ));
        }
    }
}
