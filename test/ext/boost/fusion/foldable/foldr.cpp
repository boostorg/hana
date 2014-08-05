/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/detail/constexpr.hpp>

#include "../helper.hpp"
#include <cassert>
#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x, auto xs) {
    return std::tuple_cat(std::make_tuple(x), xs);
};

int main() {
    constexpr std::tuple<> s;
    with_nonassociative_forward_sequences([=](auto container) {
        assert(foldr(container(), s, f) == s);
        assert(foldr(container(1), s, f) == f(1, s));
        assert(foldr(container(1, '2'), s, f) == f(1, f('2', s)));
        assert(foldr(container(1, '2', 3.3), s, f) == f(1, f('2', f(3.3, s))));
        assert(foldr(container(1, '2', 3.3, 4.4f), s, f) == f(1, f('2', f(3.3, f(4.4f, s)))));
    });
}
