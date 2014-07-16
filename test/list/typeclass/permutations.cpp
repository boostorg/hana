/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto permute = [=](auto xs) {
        return [=](auto ...expected_) {
            auto actual = permutations(xs);
            auto expected = list(expected_...);

            //! @todo
            //! This is a cheap unordered container membership checking.
            //! Use a real unordered container instead.
            BOOST_HANA_STATIC_ASSERT(
                length(expected) == length(actual) &&
                all([=](auto x) { return elem(x, expected); }, actual)
            );
        };
    };

    permute(list())(list());
    permute(list(x<0>))(list(x<0>));
    permute(list(x<0>, x<1>))(
        list(x<0>, x<1>),
        list(x<1>, x<0>)
    );
    permute(list(x<0>, x<1>, x<2>))(
        list(x<0>, x<1>, x<2>),
        list(x<0>, x<2>, x<1>),
        list(x<1>, x<0>, x<2>),
        list(x<1>, x<2>, x<0>),
        list(x<2>, x<0>, x<1>),
        list(x<2>, x<1>, x<0>)
    );
}

int main() {
    test<List::mcd<void>>();
}
