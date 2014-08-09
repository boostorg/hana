/*!
@file
Defines the `boost::hana::Applicative` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_APPLICATIVE_HPP
#define BOOST_HANA_DETAIL_IDENTITY_APPLICATIVE_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/detail/identity/functor.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Applicative::instance<detail::Identity<Mcd>> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x) {
            return detail::identity<Mcd>(x);
        }

        template <typename F, typename X>
        static constexpr auto ap_impl(F f, X x) {
            return x.wrap(f.value(x.value));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_APPLICATIVE_HPP
