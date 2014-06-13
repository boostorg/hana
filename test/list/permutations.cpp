/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto permute = [](auto xs) {
    return [=](auto ...expected_) {
        auto actual = permutations(xs);
        auto expected = list(expected_...);

        //! @todo
        //! Provide a real `contains` method.
        auto is_expected = [=](auto perm) {
            return any(_ == perm, expected);
        };

        //! @todo
        //! This is a cheap unordered container membership checking.
        //! Use a real unordered container instead.
        BOOST_HANA_STATIC_ASSERT(
            length(expected) == length(actual) && all(is_expected, actual)
        );
    };
};

int main() {
    permute(list_c<int>)(list_c<int>);
    permute(list_c<int, 0>)(list_c<int, 0>);
    permute(list_c<int, 0, 1>)(
        list_c<int, 0, 1>,
        list_c<int, 1, 0>
    );
    permute(list_c<int, 0, 1, 2>)(
        list_c<int, 0, 1, 2>,
        list_c<int, 0, 2, 1>,
        list_c<int, 1, 0, 2>,
        list_c<int, 1, 2, 0>,
        list_c<int, 2, 0, 1>,
        list_c<int, 2, 1, 0>
    );
}
