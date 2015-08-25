/*!
@file
Defines `boost::hana::at_key`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_AT_KEY_HPP
#define BOOST_HANA_AT_KEY_HPP

#include <boost/hana/fwd/at_key.hpp>

#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/optional.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Key>
    constexpr decltype(auto) at_key_t::operator()(Xs&& xs, Key&& key) const {
        using S = typename hana::tag_of<Xs>::type;
        using AtKey = BOOST_HANA_DISPATCH_IF(at_key_impl<S>,
            Searchable<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Searchable<S>::value,
        "hana::at_key(xs, key) requires 'xs' to be Searchable");
    #endif

        return AtKey::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
    }
    //! @endcond

    template <typename S, bool condition>
    struct at_key_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Key>
        static constexpr decltype(auto) apply(Xs&& xs, Key&& key) {
            return hana::from_just(hana::find(static_cast<Xs&&>(xs),
                                              static_cast<Key&&>(key)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_AT_KEY_HPP
