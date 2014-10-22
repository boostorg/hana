/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_MONAD_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_MONAD_HPP

// Define either one to select which MCD is used:
//  BOOST_HANA_TEST_MONAD_FLATTEN_MCD
//  BOOST_HANA_TEST_MONAD_BIND_MCD
//
// If neither is defined, the MCD used is unspecified.


#if defined(BOOST_HANA_TEST_MONAD_FLATTEN_MCD)

#include <boost/hana/monad.hpp>
#include <test/identity/identity.hpp>

// Mcd
#include <test/identity/applicative.hpp>


namespace boost { namespace hana {
    template <>
    struct Monad::instance<test::Identity>
        : Monad::flatten_mcd<test::Identity>
    {
        template <typename Id>
        static constexpr auto flatten_impl(Id self) {
            return self.value;
        }
    };
}}

#else

#include <boost/hana/monad.hpp>
#include <test/identity/identity.hpp>

// Mcd
#include <test/identity/applicative.hpp>


namespace boost { namespace hana {
    template <>
    struct Monad::instance<test::Identity>
        : Monad::bind_mcd<test::Identity>
    {
        template <typename X, typename F>
        static constexpr auto bind_impl(X x, F f) {
            return f(x.value);
        }
    };
}}

#endif

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_MONAD_HPP
