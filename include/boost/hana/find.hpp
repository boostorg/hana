/*!
@file
Defines `boost::hana::find`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FIND_HPP
#define BOOST_HANA_FIND_HPP

#include <boost/hana/fwd/find.hpp>

#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find_if.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Key>
    constexpr auto find_t::operator()(Xs&& xs, Key&& key) const {
        using S = typename hana::tag_of<Xs>::type;
        using Find = BOOST_HANA_DISPATCH_IF(find_impl<S>,
            Searchable<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Searchable<S>::value,
        "hana::find(xs, key) requires 'xs' to be Searchable");
    #endif

        return Find::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
    }
    //! @endcond

    template <typename S, bool condition>
    struct find_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Key>
        static constexpr auto apply(Xs&& xs, Key&& key) {
            return hana::find_if(static_cast<Xs&&>(xs),
                    hana::equal.to(static_cast<Key&&>(key)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FIND_HPP
