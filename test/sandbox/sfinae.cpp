/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


template <typename F>
struct _sfinae {
    F f;

    template <typename ...X>
    constexpr auto call(int, X ...x) const -> decltype(just(f(x...)))
    { return just(f(x...)); }

    template <typename ...X>
    constexpr auto call(long, X...) const
    { return nothing; }

    template <typename ...X>
    constexpr auto operator()(X ...x) const
    { return call(int{0}, x...); }
};

BOOST_HANA_CONSTEXPR_LAMBDA auto sfinae = [](auto f) {
    return _sfinae<decltype(f)>{f};
};


int main() {
    auto f = sfinae([](auto x) -> decltype(x + 1) {
        return x + 1;
    });

    BOOST_HANA_CONSTEXPR_ASSERT(f(1) == just(2));
    BOOST_HANA_CONSTANT_ASSERT(f(std::string{"abc"}) == nothing);

    BOOST_HANA_CONSTEXPR_ASSERT(bind(just(1), f) == just(2));
}
