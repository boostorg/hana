/*!
@file
Defines the `boost::hana::Monad` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_MONAD_HPP
#define BOOST_HANA_DETAIL_IDENTITY_MONAD_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/detail/identity/applicative.hpp>
#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Monad::instance<detail::Identity<Mcd>>
        : Monad::flatten_mcd<detail::Identity<Mcd>>
    {
        template <typename Id>
        static constexpr auto flatten_impl(Id self) {
            return self.value;
        }
    };

    template <>
    struct Monad::instance<detail::Identity<detail::quote<Monad::bind_mcd>>>
        : Monad::bind_mcd<detail::Identity<detail::quote<Monad::bind_mcd>>>
    {
        template <typename X, typename F>
        static constexpr auto bind_impl(X x, F f) {
            return f(x.value);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_MONAD_HPP
