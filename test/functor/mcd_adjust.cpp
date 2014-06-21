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
    struct Functor<MinimalFunctor>
        : FunctorMcd
        , defaults<Functor>::with<MinimalFunctor>
    {
        using FunctorMcd::adjust_impl;
        using defaults<hana::Functor>::with<MinimalFunctor>::fmap_impl;
    };
}}

int main() {
    FunctorTest(functor);
}
