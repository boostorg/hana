/*!
@file
Defines the `boost::hana::Searchable` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_SEARCHABLE_HPP
#define BOOST_HANA_DETAIL_IDENTITY_SEARCHABLE_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/searchable/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Searchable::instance<detail::Identity<Mcd>>
        : Searchable::mcd
    {
        template <typename Id, typename Pred>
        static constexpr auto any_impl(Id self, Pred p) {
            return p(self.value);
        }

        template <typename Id, typename Pred>
        static constexpr auto find_impl(Id self, Pred p) {
            return if_(p(self.value), just(self.value), nothing);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_SEARCHABLE_HPP
