/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monad/detail/laws.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<Mcd>;
    constexpr auto x = detail::number<>;

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(flatten(list(list(), list())) == list());
        BOOST_HANA_CONSTEXPR_ASSERT(flatten(list(list(x(0)), list())) == list(x(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(flatten(list(list(), list(x(0)))) == list(x(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(flatten(list(list(x(0)), list(x(1)))) == list(x(0), x(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(
            flatten(list(list(x(0), x(1)), list(), list(x(2), x(3)), list(x(4))))
            ==
            list(x(0), x(1), x(2), x(3), x(4))
        );
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(list, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(list, detail::injection([]{}));

        BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check(
            boost::hana::list(
                list(),
                list(x(0)),
                list(x(0), x(1)),
                list(x(0), x(1), x(2))
            ),
            boost::hana::list(
                x(0), x(1), x(2), x(3)
            ),
            boost::hana::list(f),
            boost::hana::list(g)
        ));
    }
}

int main() {
    test<List::mcd<void>>();
}
