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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto xs, auto x) {
    return std::tuple_cat(xs, std::make_tuple(x));
};

int main() {
    constexpr std::tuple<> s;
    with_nonassociative_forward_sequences([=](auto container) {
        assert(foldl(f, s, container()) == s);
        assert(foldl(f, s, container(1)) == f(s, 1));
        assert(foldl(f, s, container(1, '2')) == f(f(s, 1), '2'));
        assert(foldl(f, s, container(1, '2', 3.3)) == f(f(f(s, 1), '2'), 3.3));
        assert(foldl(f, s, container(1, '2', 3.3, 4.4f)) == f(f(f(f(s, 1), '2'), 3.3), 4.4f));
    });
}
