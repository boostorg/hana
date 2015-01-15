/*!
@file
Defines `boost::hana::Ring`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_HPP
#define BOOST_HANA_RING_HPP

#include <boost/hana/fwd/ring.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_common.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `mult`.
        //! @relates boost::hana::Ring
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Ring, datatype_t<X>>::value ||
            enable_operators<Ring, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator*(X&& x, Y&& y) {
            return mult(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }
    }

    template <typename R, typename Context>
    struct power_impl<R, when<
        is_implemented<mult_impl<R, R>, Context> &&
        is_implemented<one_impl<R>, Context>
    >, Context> {
        template <typename X, typename P>
        static constexpr decltype(auto) apply(X&& x, P&& p) {
            using Exp = datatype_t<P>;
            return eval_if(equal(p, zero<Exp>()),
                always(one<R>()),
                [&p, &x](auto _) -> decltype(auto) {
                    return mult(
                        x, apply(x, _(pred)(detail::std::forward<P>(p)))
                    );
                }
            );
        }
    };

    template <typename T>
    struct mult_impl<T, T, when_valid<
        decltype(detail::std::declval<T>() * detail::std::declval<T>())
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return detail::std::forward<X>(x) * detail::std::forward<Y>(y);
        }
    };

    template <typename T>
    struct one_impl<T, when_valid<decltype(static_cast<T>(1))>> {
        static constexpr T apply()
        { return static_cast<T>(1); }
    };

    BOOST_HANA_DISPATCH_COMMON(mult, mult_impl, Ring);

    template <typename R>
    constexpr auto is_a<Ring, R> = bool_<
        is_a<Group, R>() &&
        is_implemented<mult_impl<R, R>> &&
        is_implemented<one_impl<R>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_HPP
