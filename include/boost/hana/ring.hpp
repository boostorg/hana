/*!
@file
Defines `boost::hana::Ring`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_HPP
#define BOOST_HANA_RING_HPP

#include <boost/hana/fwd/ring.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>

// for default power
#include <boost/hana/comparable.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(mult)>::value ||
            has_operator<datatype_t<Y>, decltype(mult)>::value
        >::type>
        constexpr decltype(auto) operator*(X&& x, Y&& y) {
            return hana::mult(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // mult
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct mult_impl : mult_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct mult_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr void apply(X&&, Y&&) {
            static_assert(wrong<mult_impl<T, U>, X, Y>{},
            "no definition of boost::hana::mult for the given data types");
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct mult_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Ring, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::mult(to<C>(detail::std::forward<X>(x)),
                              to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // one
    //////////////////////////////////////////////////////////////////////////
    template <typename R, typename>
    struct one_impl : one_impl<R, when<true>> { };

    template <typename R, bool condition>
    struct one_impl<R, when<condition>> : default_ {
        template <typename ...Nothing>
        static constexpr void apply(Nothing&& ...) {
            static_assert(wrong<one_impl<R>, Nothing...>{},
            "no definition of boost::hana::one for the given data type");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // power
    //////////////////////////////////////////////////////////////////////////
    template <typename R, typename>
    struct power_impl : power_impl<R, when<true>> { };

    template <typename R, bool condition>
    struct power_impl<R, when<condition>> : default_ {
        template <typename X, typename N>
        static constexpr decltype(auto) apply(X&& x, N&& n) {
            using Exp = typename datatype<N>::type;
            return eval_if(equal(n, zero<Exp>()),
                always(one<R>()),
                [&n, &x](auto _) -> decltype(auto) {
                    return mult(
                        x, apply(x, _(pred)(detail::std::forward<N>(n)))
                    );
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct models<Ring(R)>
        : detail::std::integral_constant<bool,
            !is_default<one_impl<R>>{} &&
            !is_default<mult_impl<R, R>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct mult_impl<T, T, when<detail::std::is_non_boolean_arithmetic<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) * detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct one_impl<T, when<detail::std::is_non_boolean_arithmetic<T>{}>> {
        static constexpr T apply()
        { return static_cast<T>(1); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Ring
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct mult_impl<C, C, when<
        models<Constant(C)>{} && models<Ring(typename C::value_type)>{}
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::mult(boost::hana::value<X>(),
                                         boost::hana::value<Y>());
            }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return to<C>(_constant<X, Y>{}); }
    };

    template <typename C>
    struct one_impl<C, when<
        models<Constant(C)>{} && models<Ring(typename C::value_type)>{}
    >> {
        using T = typename C::value_type;
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::one<T>(); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        static constexpr decltype(auto) apply()
        { return to<C>(_constant{}); }
    };

}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_HPP
