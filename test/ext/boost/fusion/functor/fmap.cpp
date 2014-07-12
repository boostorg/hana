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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return std::make_tuple(x);
};

int main() {
    with_nonassociative_forward_sequences([](auto container) {
        assert(fmap(f, container()) == container());
        assert(fmap(f, container(1)) == container(f(1)));
        assert(fmap(f, container(1, '2')) == container(f(1), f('2')));
        assert(fmap(f, container(1, '2', 3.3)) == container(f(1), f('2'), f(3.3)));
    });
}
