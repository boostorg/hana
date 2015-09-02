/*!
@file
Defines `boost::hana::pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/fwd/pair.hpp>

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/first.hpp>
#include <boost/hana/fwd/second.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename First, typename Second>
    struct pair : operators::adl, detail::closure<First, Second> {
        using detail::closure<First, Second>::closure;
        using hana_tag = pair_tag;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<pair_tag> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<pair_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<pair_tag> {
        template <typename F, typename S>
        static constexpr pair<
            typename std::decay<F>::type,
            typename std::decay<S>::type
        > apply(F&& f, S&& s) {
            return {static_cast<F&&>(f), static_cast<S&&>(s)};
        }
    };

    template <>
    struct first_impl<pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return detail::get<0>(static_cast<P&&>(p)); }
    };

    template <>
    struct second_impl<pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return detail::get<1>(static_cast<P&&>(p)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
