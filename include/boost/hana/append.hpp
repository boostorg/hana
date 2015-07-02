/*!
@file
Defines `boost::hana::append`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPEND_HPP
#define BOOST_HANA_APPEND_HPP

#include <boost/hana/fwd/append.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/lift.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename X>
    constexpr auto append_t::operator()(Xs&& xs, X&& x) const {
        using M = typename datatype<Xs>::type;
        using Append = BOOST_HANA_DISPATCH_IF(append_impl<M>,
            _models<MonadPlus, M>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<MonadPlus, M>::value,
        "hana::append(xs, x) requires 'xs' to be a MonadPlus");
    #endif

        return Append::apply(static_cast<Xs&&>(xs), static_cast<X&&>(x));
    }
    //! @endcond

    template <typename M, bool condition>
    struct append_impl<M, when<condition>> : default_ {
        template <typename Xs, typename X>
        static constexpr auto apply(Xs&& xs, X&& x) {
            return hana::concat(static_cast<Xs&&>(xs),
                                hana::lift<M>(static_cast<X&&>(x)));
        }
    };

    template <typename S>
    struct append_impl<S, when<_models<Sequence, S>::value>> {
        template <typename Xs, typename X, std::size_t ...i>
        static constexpr auto append_helper(Xs&& xs, X&& x, std::index_sequence<i...>) {
            return hana::make<S>(
                hana::at_c<i>(static_cast<Xs&&>(xs))..., static_cast<X&&>(x)
            );
        }

        template <typename Xs, typename X>
        static constexpr auto apply(Xs&& xs, X&& x) {
            constexpr std::size_t N = hana::value<decltype(hana::length(xs))>();
            return append_helper(static_cast<Xs&&>(xs), static_cast<X&&>(x),
                                 std::make_index_sequence<N>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPEND_HPP
