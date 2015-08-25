/*!
@file
Defines `boost::hana::drop_back`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_BACK_HPP
#define BOOST_HANA_DROP_BACK_HPP

#include <boost/hana/fwd/drop_back.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto drop_back_t::operator()(Xs&& xs, N&& n) const {
        using S = typename datatype<Xs>::type;
        using DropBack = BOOST_HANA_DISPATCH_IF(drop_back_impl<S>,
            _models<Sequence, S>::value &&
            _models<Constant, N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::drop_back(xs, n) requires 'xs' to be a Sequence");

        static_assert(_models<Constant, N>::value,
        "hana::drop_back(xs, n) requires 'n' to be a Constant");
    #endif

        return DropBack::apply(static_cast<Xs&&>(xs), static_cast<N&&>(n));
    }

    template <typename Xs>
    constexpr auto drop_back_t::operator()(Xs&& xs) const {
        return (*this)(static_cast<Xs&&>(xs), hana::size_t<1>);
    }
    //! @endcond

    template <typename S, bool condition>
    struct drop_back_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...n>
        static constexpr auto drop_back_helper(Xs&& xs, std::index_sequence<n...>) {
            return hana::make<S>(hana::at_c<n>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N&&) {
            constexpr std::size_t n = hana::value<N>();
            constexpr std::size_t len = hana::value<decltype(hana::length(xs))>();
            return drop_back_helper(static_cast<Xs&&>(xs),
                                    std::make_index_sequence<(n > len ? 0 : len - n)>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DROP_BACK_HPP
