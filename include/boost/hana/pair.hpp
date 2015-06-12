/*!
@file
Defines `boost::hana::Pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/fwd/pair.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/product.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _pair
    //////////////////////////////////////////////////////////////////////////
    template <typename First, typename Second>
    struct _pair : operators::adl {
        First first;
        Second second;

        constexpr _pair() : first{}, second{} { }

        template <typename F, typename S, typename = decltype(
            ((void)First(std::declval<F>())),
            ((void)Second(std::declval<S>()))
        )>
        constexpr _pair(F&& f, S&& s)
            : first(static_cast<F&&>(f))
            , second(static_cast<S&&>(s))
        { }

        using hana = _pair;
        using datatype = Pair;
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <>
        struct of<Pair>
            : operators::of<Comparable, Orderable>
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Pair> {
        template <typename F, typename S>
        static constexpr _pair<
            typename std::decay<F>::type,
            typename std::decay<S>::type
        > apply(F&& f, S&& s) {
            return {static_cast<F&&>(f), static_cast<S&&>(s)};
        }
    };

    template <>
    struct first_impl<Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).first; }
    };

    template <>
    struct second_impl<Pair> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return static_cast<P&&>(p).second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
