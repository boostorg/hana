/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto fact = fix(
    [](auto fact, auto n) {
        return if_(n == int_<0>,
            always(int_<1>),
            [](auto fact, auto n) { return n * fact(n - int_<1>); }
        )(fact, n);
    }
);

constexpr unsigned long long fact_test(unsigned long long n)
{ return n == 0 ? 1 : n * fact_test(n - 1); }


int main() {
    //! @todo Use some kind of monadic mapping here.
    fmap(
        [](auto n) {
            BOOST_HANA_STATIC_ASSERT(fact(n) == fact_test(n));
            return 0;
        },
        range(size_t<0>, size_t<15>)
    );
}
