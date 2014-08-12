/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_APPLICATIVE_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <test/identity/identity.hpp>

// Mcd
#include <test/identity/functor.hpp>


namespace boost { namespace hana {
    template <>
    struct Applicative::instance<test::Identity>
        : Applicative::mcd
    {
        template <typename X>
        static constexpr auto lift_impl(X x) {
            return test::identity(x);
        }

        template <typename F, typename X>
        static constexpr auto ap_impl(F f, X x) {
            return test::identity(f.value(x.value));
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_APPLICATIVE_HPP
