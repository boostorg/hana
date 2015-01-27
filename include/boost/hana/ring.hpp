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

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
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
        //! Equivalent to `mult`.
        //! @relates boost::hana::Ring
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Ring, datatype_t<X>>::value ||
            enable_operators<Ring, datatype_t<Y>>::value
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
    struct mult_impl<T, U, when<condition>> {
        static_assert(wrong<mult_impl<T, U>>{},
        "no definition of boost::hana::mult for the given data types");
    };

    // Cross-type overload
    template <typename T, typename U>
    struct mult_impl<T, U, when<detail::has_common_embedding<Ring, T, U>{}>> {
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
    struct one_impl<R, when<condition>> {
        static_assert(wrong<one_impl<R>>{},
        "no definition of boost::hana::one for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // power
    //////////////////////////////////////////////////////////////////////////
    template <typename R, typename>
    struct power_impl : power_impl<R, when<true>> { };

    template <typename R, bool condition>
    struct power_impl<R, when<condition>> {
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
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Ring(T), when<detail::std::is_arithmetic<T>{}>>
        : detail::std::true_type
    { };

    template <typename T>
    struct mult_impl<T, T, when<detail::std::is_arithmetic<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) * detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct one_impl<T, when<detail::std::is_arithmetic<T>{}>> {
        static constexpr T apply()
        { return static_cast<T>(1); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_HPP
