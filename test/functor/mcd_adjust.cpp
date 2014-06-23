/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor.hpp>

#include <boost/hana/logical.hpp>

#include "minimal.hpp"
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Functor::instance<MinimalFunctor> : Functor::adjust_mcd {
        template <typename Pred, typename F, typename Functor>
        static constexpr auto adjust_impl(Pred p, F f, Functor self) {
            return self.storage([=](auto ...x) {
                return functor(
                    eval_if(p(x),
                        [=](auto _) { return _(f)(x); },
                        [=](auto _) { return x; }
                    )...
                );
            });
        }
    };
}}

int main() {
    FunctorTest(functor);
}
