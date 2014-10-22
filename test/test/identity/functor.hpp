/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_FUNCTOR_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_FUNCTOR_HPP

// Define either one to select which MCD is used:
//  BOOST_HANA_TEST_FUNCTOR_FMAP_MCD
//  BOOST_HANA_TEST_FUNCTOR_ADJUST_MCD_MCD
//
// If neither is defined, the MCD used is unspecified.


#if defined(BOOST_HANA_TEST_FUNCTOR_FMAP_MCD)

#include <boost/hana/functor.hpp>
#include <test/identity/identity.hpp>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<test::Identity>
        : Functor::fmap_mcd
    {
        template <typename Id, typename F>
        static constexpr auto fmap_impl(Id self, F f) {
            return test::identity(f(self.value));
        }
    };
}}

#else

#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <test/identity/identity.hpp>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<test::Identity>
        : Functor::adjust_mcd
    {
        template <typename Id, typename P, typename F>
        static constexpr auto adjust_impl(Id self, P p, F f) {
            auto x = eval_if(p(self.value),
                [=](auto _) { return _(f)(self.value); },
                [=](auto _) { return self.value; }
            );
            return test::identity(x);
        }
    };
}}

#endif

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_FUNCTOR_HPP
