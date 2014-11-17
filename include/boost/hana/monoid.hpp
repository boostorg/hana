/*!
@file
Defines `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_HPP
#define BOOST_HANA_MONOID_HPP

#include <boost/hana/fwd/monoid.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `zero` and `plus`
    struct Monoid::mcd { };

    namespace operators {
        //! Equivalent to `plus`.
        //! @relates boost::hana::Monoid
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Monoid, datatype_t<X>>::value ||
            enable_operators<Monoid, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator+(X&& x, Y&& y) {
            return plus(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }
    }

    template <typename T, typename U>
    struct Monoid::default_instance
        : Monoid::instance<common_t<T, U>, common_t<T, U>>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) plus_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return plus(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_HPP
