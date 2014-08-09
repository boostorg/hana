/*!
@file
Defines the `boost::hana::Foldable` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_FOLDABLE_HPP
#define BOOST_HANA_DETAIL_IDENTITY_FOLDABLE_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Foldable::instance<detail::Identity<Mcd>>
        : Foldable::unpack_mcd
    {
        template <typename Id, typename F>
        static constexpr auto unpack_impl(Id self, F f) {
            return f(self.value);
        }
    };

    template <>
    struct Foldable::instance<detail::Identity<Foldable::mcd>>
        : Foldable::mcd
    {
        template <typename Id, typename S, typename F>
        static constexpr auto foldl_impl(Id self, S s, F f) {
            return f(s, self.value);
        }

        template <typename Id, typename S, typename F>
        static constexpr auto foldr_impl(Id self, S s, F f) {
            return f(self.value, s);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_FOLDABLE_HPP
