/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor/laws.hpp>

#include <cassert>
#include <list>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + 1; };
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x * 3; };

int main() {
    using std_list = std::list<int>;
    assert(fmap(f, std_list{}) == std_list{});
    assert(fmap(f, std_list{1}) == std_list{2});
    assert(fmap(f, std_list{1, 2}) == (std_list{2, 3}));
    assert(fmap(f, std_list{1, 2, 3}) == (std_list{2, 3, 4}));

#if 0
    assert(Functor::laws::check(std_list{}, f, g));
    assert(Functor::laws::check(std_list{1}, f, g));
    assert(Functor::laws::check(std_list{1, 2, 3}, f, g));
#endif
}
