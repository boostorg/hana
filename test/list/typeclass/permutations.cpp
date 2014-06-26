/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>

#include "minimal.hpp"
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto permute = [](auto xs) {
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

int main() {
    permute(ilist<>)(ilist<>);
    permute(ilist<0>)(ilist<0>);
    permute(ilist<0, 1>)(
        ilist<0, 1>,
        ilist<1, 0>
    );
    permute(ilist<0, 1, 2>)(
        ilist<0, 1, 2>,
        ilist<0, 2, 1>,
        ilist<1, 0, 2>,
        ilist<1, 2, 0>,
        ilist<2, 0, 1>,
        ilist<2, 1, 0>
    );
}
