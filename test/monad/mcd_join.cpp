/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad.hpp>

#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include "minimal.hpp"
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Monad<MinimalMonad>
        : MonadInstance
        , defaults<Monad>::with<MinimalMonad>
    {
        using MonadInstance::join_impl;
        using defaults<hana::Monad>::with<MinimalMonad>::bind_impl;
    };
}}

int main() {
    BOOST_HANA_STATIC_ASSERT(detail::laws<Monad>(
        monad<int>{1},
        2,
        [](auto x) { return monad<int>{x + 1}; },
        [](auto x) { return monad<int>{x * 2}; }
    ));
}
