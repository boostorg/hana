/*!
@file
Defines `boost::hana::take_while`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TAKE_WHILE_HPP
#define BOOST_HANA_TAKE_WHILE_HPP

#include <boost/hana/fwd/take_while.hpp>

#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/first_unsatisfied_index.hpp>
#include <boost/hana/take.hpp>
#include <boost/hana/unpack.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto take_while_t::operator()(Xs&& xs, Pred&& pred) const {
        using S = typename hana::tag_of<Xs>::type;
        using TakeWhile = BOOST_HANA_DISPATCH_IF(take_while_impl<S>,
            Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Sequence<S>::value,
        "hana::take_while(xs, pred) requires 'xs' to be a Sequence");
    #endif

        return TakeWhile::apply(static_cast<Xs&&>(xs),
                                static_cast<Pred&&>(pred));
    }
    //! @endcond

    template <typename S, bool condition>
    struct take_while_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&&) {
            using FirstUnsatisfied = decltype(
                hana::unpack(static_cast<Xs&&>(xs),
                             detail::first_unsatisfied_index<Pred&&>{})
            );
            return hana::take(static_cast<Xs&&>(xs), FirstUnsatisfied{});
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_TAKE_WHILE_HPP
