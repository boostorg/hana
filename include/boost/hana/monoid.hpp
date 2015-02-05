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
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
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

    //! Instance of `Monoid` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that can be added with the usual `operator+`
    //! and for which a valid conversion from `int` exists (for both)
    //! naturally form an additive `Monoid`, with `0` being the identity
    //! and the usual `operator+` being the associative operation.
    template <typename T, typename U>
    struct Monoid::instance<T, U, when_valid<
        decltype(static_cast<T>(0)),
        decltype(static_cast<U>(0)),
        decltype(detail::std::declval<T>() + detail::std::declval<U>())
    >> : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) plus_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) + detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr decltype(auto) zero_impl()
        { return static_cast<T>(0); }
    };

    template <typename T>
    struct Monoid::instance<T, T, when<models<Monoid(T)>{}>>
        : Monoid::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_HPP
