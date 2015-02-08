/*!
@file
Defines `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_INTEGRAL_CONSTANT_HPP

#include <boost/hana/fwd/integral_constant.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/functional/id.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `Constant` and `integral_constant`.
    struct IntegralConstant::mcd { };

    template <template <typename ...> class C1, typename T,
              template <typename ...> class C2, typename U>
    struct Comparable::integral_constant_mcd<C1<T>, C2<U>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X x, Y y) {
            constexpr auto eq = value(x) == value(y);
            return integral_constant<C1<decltype(eq)>, eq>();
        }
    };

    template <typename I1, typename I2>
    struct Comparable::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : Comparable::integral_constant_mcd<I1, I2>
    { };

    //! Instance of `Logical` for `IntegralConstant`s.
    //!
    //! An `IntegralConstant` is true-valued if and only if its integral
    //! value is true-valued, i.e. if
    //! @code
    //!     static_cast<bool>(value(n)) == true
    //! @endcode
    template <template <typename ...> class C, typename T>
    struct Logical::integral_constant_mcd<C<T>> : Logical::mcd {
        template <bool> struct tval { };

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(tval<true>, Then t, Else e)
        { return t(id); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(tval<false>, Then t, Else e)
        { return e(id); }

        template <typename Cond, typename Then, typename Else>
        static constexpr auto eval_if_impl(Cond c, Then t, Else e)
        { return eval_if_impl(tval<static_cast<bool>(value(c))>{}, t, e); }

        template <typename Cond>
        static constexpr auto not_impl(Cond c) {
            constexpr auto nc = !value(c);
            return integral_constant<C<decltype(nc)>, nc>();
        }

        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(tval<false>, Pred&& pred, State&& state, F&& f) {
            return detail::std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(tval<true>, Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(detail::std::forward<State>(state));
            return while_(detail::std::forward<Pred>(pred),
                          detail::std::forward<decltype(r)>(r),
                          detail::std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_impl(Pred&& pred, State&& state, F&& f) {
            auto cond = pred(state);
            return while_helper(tval<static_cast<bool>(value(cond))>{},
                                detail::std::forward<Pred>(pred),
                                detail::std::forward<State>(state),
                                detail::std::forward<F>(f));
        }
    };

    template <typename I>
    struct Logical::instance<I, when<is_an<IntegralConstant, I>()>>
        : Logical::integral_constant_mcd<I>
    { };

    template <template <typename ...> class C1, typename T,
              template <typename ...> class C2, typename U>
    struct Orderable::integral_constant_mcd<C1<T>, C2<U>>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y) {
            constexpr auto ord = value(x) < value(y);
            return integral_constant<C1<decltype(ord)>, ord>();
        }
    };

    template <typename I1, typename I2>
    struct Orderable::instance<I1, I2, when<
        is_an<IntegralConstant, I1>() &&
        is_an<IntegralConstant, I2>()
    >>
        : Orderable::integral_constant_mcd<I1, I2>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_CONSTANT_HPP
