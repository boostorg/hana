/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/functor/laws.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <int i, int j>
struct _f {
    template <typename X>
    constexpr auto operator()(X x) const
    { return std::make_tuple(i, j, x); }
};

template <int i, int j>
constexpr _f<i, j> f{};

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
        list(
            list(),
            list(x<0>),
            list(x<0>, x<1>),
            list(x<0>, x<1>, x<2>)
        ),
        list(
            f<1, 1>,
            f<1, 2>
        ),
        list(
            f<2, 1>,
            f<2, 2>
        )
    ));
}

int main() {
    test<List::mcd<void>>();
}
