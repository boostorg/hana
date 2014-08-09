/*!
@file
Defines the `boost::hana::Traversable` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_TRAVERSABLE_HPP
#define BOOST_HANA_DETAIL_IDENTITY_TRAVERSABLE_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/detail/identity/functor.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Traversable::instance<detail::Identity<Mcd>>
        : Traversable::traverse_mcd
    {
        template <typename A, typename F, typename Id>
        static constexpr auto traverse_impl(F f, Id self) {
            return fmap([=](auto x) { return self.wrap(x); }, f(self.value));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_TRAVERSABLE_HPP
