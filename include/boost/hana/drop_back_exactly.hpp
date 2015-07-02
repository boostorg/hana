/*!
@file
Defines `boost::hana::drop_back_exactly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DROP_BACK_EXACTLY_HPP
#define BOOST_HANA_DROP_BACK_EXACTLY_HPP

#include <boost/hana/fwd/drop_back_exactly.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename N>
    constexpr auto drop_back_exactly_t::operator()(Xs&& xs, N&& n) const {
        using S = typename datatype<Xs>::type;
        using DropBackExactly = BOOST_HANA_DISPATCH_IF(drop_back_exactly_impl<S>,
            _models<Sequence, S>::value &&
            _models<Constant, N>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::drop_back_exactly(xs, n) requires 'xs' to be a Sequence");

        static_assert(_models<Constant, N>::value,
        "hana::drop_back_exactly(xs, n) requires 'n' to be a Constant");
    #endif

        return DropBackExactly::apply(static_cast<Xs&&>(xs),
                                      static_cast<N&&>(n));
    }

    template <typename Xs>
    constexpr auto drop_back_exactly_t::operator()(Xs&& xs) const {
        return (*this)(static_cast<Xs&&>(xs), hana::size_t<1>);
    }
    //! @endcond

    template <typename S, bool condition>
    struct drop_back_exactly_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...n>
        static constexpr auto
        drop_back_exactly_helper(Xs&& xs, std::index_sequence<n...>) {
            return hana::make<S>(
                hana::at(static_cast<Xs&&>(xs), hana::size_t<n>)...
            );
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N&&) {
            constexpr std::size_t n = hana::value<N>();
            constexpr std::size_t len = hana::value<decltype(hana::length(xs))>();
            return drop_back_exactly_helper(
                static_cast<Xs&&>(xs), std::make_index_sequence<len - n>{}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DROP_BACK_EXACTLY_HPP
