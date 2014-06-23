/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor.hpp>

#include "minimal.hpp"
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Functor::instance<MinimalFunctor> : Functor::fmap_mcd {
        template <typename F, typename Functor>
        static constexpr auto fmap_impl(F f, Functor self)
        { return self.storage([=](auto ...x) { return functor(f(x)...); }); }
    };
}}

int main() {
    FunctorTest(functor);
}
