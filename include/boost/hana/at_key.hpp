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

#include <boost/hana/accessors.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/concept/struct.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/second.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename Key>
    constexpr decltype(auto) at_key_t::operator()(Xs&& xs, Key&& key) const {
        using S = typename hana::tag_of<Xs>::type;
        using AtKey = BOOST_HANA_DISPATCH_IF(at_key_impl<S>,
            hana::Searchable<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Searchable<S>::value,
        "hana::at_key(xs, key) requires 'xs' to be Searchable");
    #endif

        return AtKey::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
    }
    //! @endcond

    template <typename S, bool condition>
    struct at_key_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Key>
        static constexpr auto apply(Xs&& xs, Key&& key) {
            return hana::find(static_cast<Xs&&>(xs), static_cast<Key&&>(key)).value();
        }
    };

    template <typename S>
    struct at_key_impl<S, when<hana::Struct<S>::value>> {
        template <typename X, typename Key>
        static constexpr decltype(auto) apply(X&& x, Key const& key) {
            auto accessor = hana::second(*hana::find_if(hana::accessors<S>(),
                hana::equal.to(key) ^hana::on^ hana::first
            ));
            return accessor(static_cast<X&&>(x));
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_AT_KEY_HPP
