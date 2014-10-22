/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_ITERABLE_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>

#include <test/injection.hpp>
#include <test/laws/iterable.hpp>


namespace iterable_detail {
    template <int i>
    struct _non_pod { virtual ~_non_pod() { } };

    template <int i = 0>
    _non_pod<i> non_pod{};


    template <int i>
    struct _invalid { };

    template <int i = 0>
    constexpr _invalid<i> invalid{};
}

template <typename L>
void List_iterable() {
    using namespace iterable_detail;
    using namespace boost::hana;

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = make<L>;
    using test::x;

    // head
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(head(list(x<0>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(list(x<0>, invalid<>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(head(list(x<0>, invalid<1>, invalid<2>)), x<0>));
    }

    // is_empty
    {
        BOOST_HANA_CONSTANT_ASSERT(is_empty(list()));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(list(invalid<>))));
        BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(list(invalid<0>, invalid<1>))));
    }

    // tail
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(list(invalid<>, x<0>)), list(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(list(invalid<>, x<0>, x<1>)), list(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(tail(list(invalid<>, x<0>, x<1>, x<2>)), list(x<0>, x<1>, x<2>)));
    }

    // at
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<0>, list(x<0>)), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<0>, list(x<0>, invalid<>)), x<0>));

        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<1>, list(invalid<>, x<1>)), x<1>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<1>, list(invalid<0>, x<1>, invalid<2>)), x<1>));

        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<2>, list(invalid<0>, invalid<1>, x<2>)), x<2>));
        BOOST_HANA_CONSTANT_ASSERT(equal(at(size_t<2>, list(invalid<0>, invalid<1>, x<2>, invalid<3>)), x<2>));

        // make sure we can use non-pods on both sides
        at(size_t<1>, list(non_pod<0>, x<1>, non_pod<2>));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Iterable_laws(
            list(),
            list(x<0>),
            list(x<0>, x<1>),
            list(x<0>, x<1>, x<2>)
        ));
    }
}

#endif // !BOOST_HANA_TEST_TEST_ISO_LIST_ITERABLE_HPP
