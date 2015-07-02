/*!
@file
Defines `boost::hana::take_until`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TAKE_UNTIL_HPP
#define BOOST_HANA_TAKE_UNTIL_HPP

#include <boost/hana/fwd/take_until.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/functional/compose.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/take_while.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto take_until_t::operator()(Xs&& xs, Pred&& pred) const {
        using S = typename datatype<Xs>::type;
        using TakeUntil = BOOST_HANA_DISPATCH_IF(take_until_impl<S>,
            _models<Sequence, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::take_until(xs, pred) requires 'xs' to be a Sequence");
    #endif

        return TakeUntil::apply(static_cast<Xs&&>(xs),
                                static_cast<Pred&&>(pred));
    }
    //! @endcond

    template <typename S, bool condition>
    struct take_until_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return hana::take_while(
                static_cast<Xs&&>(xs),
                hana::compose(hana::not_, static_cast<Pred&&>(pred))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TAKE_UNTIL_HPP
