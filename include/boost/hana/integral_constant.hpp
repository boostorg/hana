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
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/common_type.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
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
    template <typename U, template <typename ...> class C, typename T>
    struct convert<U, C<T>, when<
        is_an<IntegralConstant, C<T>>{} &&
        detail::std::is_integral<U>::value &&
        ((decltype(static_cast<U>(detail::std::declval<T>()))*)0, true)
    >> {
        template <typename X>
        static constexpr U apply(X x)
        { return static_cast<U>(value(x)); }
    };

    // Provides a common type between an `IntegralConstant` and
    // any integral type.
    template <template <typename ...> class C, typename T, typename U>
    struct common<C<T>, U, when<
        is_an<IntegralConstant, C<T>>{} &&
        detail::std::is_integral<U>::value
    >> { using type = typename detail::std::common_type<T, U>::type; };

    template <template <typename ...> class C, typename T, typename U>
    struct common<U, C<T>, when<
        is_an<IntegralConstant, C<T>>{} &&
        detail::std::is_integral<U>::value
    >> { using type = typename detail::std::common_type<T, U>::type; };

    template <template <typename ...> class C1, typename T,
              template <typename ...> class C2, typename U>
    struct common<C1<T>, C2<U>, when<
        is_an<IntegralConstant, C1<T>>{} &&
        is_an<IntegralConstant, C2<U>>{}
    >> {
        using type = C1<typename detail::std::common_type<T, U>::type>;
    };

    template <template <typename ...> class C, typename T>
    struct pred_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        static constexpr auto dec(bool x) { return static_cast<int>(x)-1; }
        template <typename X> static constexpr auto dec(X x) { return --x; }

        template <typename X>
        static constexpr decltype(auto) apply(X x) {
            constexpr auto n = value(x);
            return integral_constant<C<decltype(dec(n))>, dec(n)>();
        }
    };

    template <template <typename ...> class C, typename T>
    struct succ_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        //! @todo What should happen when we go out of bounds? Probably not
        //! overflow like it does currently. Also, `succ(false_)` should
        //! probably not be `int_<1>`, but rather `true_`.
        static constexpr auto inc(bool x) { return static_cast<int>(x)+1; }
        template <typename X> static constexpr auto inc(X x) { return ++x; }

        template <typename X>
        static constexpr decltype(auto) apply(X x) {
            constexpr auto n = value(x);
            return integral_constant<C<decltype(inc(n))>, inc(n)>();
        }
    };

#define BOOST_HANA_INTEGRAL_CONSTANT_OP(METHOD_IMPL, OP)                    \
    template <template <typename ...> class C1, typename T,                 \
              template <typename ...> class C2, typename U>                 \
    struct METHOD_IMPL<C1<T>, C2<U>, when<                                  \
        is_an<IntegralConstant, C1<T>>{} && is_an<IntegralConstant, C2<U>>{}\
    >> {                                                                    \
        template <typename X, typename Y>                                   \
        static constexpr auto apply(X x, Y y) {                             \
            constexpr auto tmp = value(x) OP value(y);                      \
            return integral_constant<C1<decltype(tmp)>, tmp>();             \
        }                                                                   \
    }                                                                       \
/**/
    BOOST_HANA_INTEGRAL_CONSTANT_OP(minus_impl, -);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(quot_impl, /);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(mod_impl, %);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(equal_impl, ==);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(less_impl, <);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(plus_impl, +);
    BOOST_HANA_INTEGRAL_CONSTANT_OP(mult_impl, *);
#undef BOOST_HANA_INTEGRAL_CONSTANT_OP

    template <template <typename ...> class C, typename T>
    struct eval_if_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        template <typename Then, typename Else>
        static constexpr auto apply(detail::std::true_type, Then t, Else e)
        { return t(id); }

        template <typename Then, typename Else>
        static constexpr auto apply(detail::std::false_type, Then t, Else e)
        { return e(id); }

        template <typename Cond, typename Then, typename Else>
        static constexpr auto apply(Cond c, Then t, Else e) {
            constexpr bool truth_value = static_cast<bool>(value(c));
            return apply(detail::std::integral_constant<bool, truth_value>{}, t, e);
        }
    };

    template <template <typename ...> class C, typename T>
    struct not_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        template <typename Cond>
        static constexpr auto apply(Cond c) {
            constexpr auto nc = !value(c);
            return integral_constant<C<decltype(nc)>, nc>();
        }
    };

    template <template <typename ...> class C, typename T>
    struct while_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(detail::std::false_type, Pred&& pred, State&& state, F&& f) {
            return detail::std::forward<State>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(detail::std::true_type, Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(detail::std::forward<State>(state));
            return while_(detail::std::forward<Pred>(pred),
                          detail::std::forward<decltype(r)>(r),
                          detail::std::forward<F>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(Pred&& pred, State&& state, F&& f) {
            auto cond = pred(state);
            constexpr bool truth_value = static_cast<bool>(value(cond));
            return while_helper(detail::std::integral_constant<bool, truth_value>{},
                                detail::std::forward<Pred>(pred),
                                detail::std::forward<State>(state),
                                detail::std::forward<F>(f));
        }
    };

    template <template <typename ...> class C, typename T>
    struct zero_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        static constexpr auto apply()
        { return integral_constant<C<T>, 0>(); }
    };

    template <template <typename ...> class C, typename T>
    struct one_impl<C<T>, when<is_an<IntegralConstant, C<T>>{}>> {
        static constexpr auto apply()
        { return integral_constant<C<T>, 1>(); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_CONSTANT_HPP
