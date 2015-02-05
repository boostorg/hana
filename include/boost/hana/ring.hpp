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

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `one` and `mult`
    struct Ring::mcd {
        template <typename X, typename P>
        static constexpr decltype(auto) power_impl(X&& x, P&& p) {
            using R = datatype_t<X>;
            using E = datatype_t<P>;
            return eval_if(equal(p, zero<E>()),
                always(one<R>()),
                [&p, &x](auto _) -> decltype(auto) {
                    return mult(
                        x, power_impl(x, _(pred)(detail::std::forward<P>(p)))
                    );
                }
            );
        }
    };

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

    template <typename T, typename U>
    struct Ring::default_instance
        : Ring::instance<common_t<T, U>, common_t<T, U>>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) mult_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return mult(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };

    //! Instance of `Ring` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that are `Group`s, that can be multiplied
    //! with the usual `operator*` and for which a valid conversion from `int`
    //! exists (for both) naturally form a multiplicative `Ring`, with `1`
    //! being the identity and the usual `operator*` being the ring operation.
    template <typename T, typename U>
    struct Ring::instance<T, U, when_valid<
        decltype(static_cast<T>(1)),
        decltype(static_cast<U>(1)),
        decltype(detail::std::declval<T>() * detail::std::declval<U>()),
        char[are<Group, T, U>()]
    >> : Ring::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) mult_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) * detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr auto one_impl()
        { return static_cast<T>(1); }
    };

    template <typename T>
    struct Ring::instance<T, T, when<models<Ring(T)>{}>>
        : Ring::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_HPP
