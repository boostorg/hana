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
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `one` and `mult`
    struct Ring::mcd {
        template <typename X, typename P>
        static constexpr decltype(auto) power_impl(X&& x, P&& p) {
            using R = datatype_t<X>;
            using E = datatype_t<P>;
            return eval_if(equal(p, zero<E>),
                always(one<R>),
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_HPP
