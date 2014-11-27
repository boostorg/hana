/*!
@file
Defines `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_HPP
#define BOOST_HANA_GROUP_HPP

#include <boost/hana/fwd/group.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `minus`
    template <typename G1, typename G2>
    struct Group::minus_mcd {
        template <typename X>
        static constexpr auto negate_impl(X x) {
            return minus(zero<G1>(), x);
        }
    };

    //! Minimal complete definition: `negate`
    template <typename G1, typename G2>
    struct Group::negate_mcd {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y) {
            return plus(x, negate(y));
        }
    };

    namespace operators {
        //! Equivalent to `minus`.
        //! @relates boost::hana::Group
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Group, datatype_t<X>>::value ||
            enable_operators<Group, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator-(X&& x, Y&& y) {
            return minus(detail::std::forward<X>(x),
                         detail::std::forward<Y>(y));
        }

        //! Equivalent to `negate`.
        //! @relates boost::hana::Group
        template <typename X, typename = detail::std::enable_if_t<
            enable_operators<Group, datatype_t<X>>::value
        >>
        constexpr decltype(auto) operator-(X&& x)
        { return negate(detail::std::forward<X>(x)); }
    }

    template <typename T, typename U>
    struct Group::default_instance
        : Group::instance<common_t<T, U>, common_t<T, U>>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) minus_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return minus(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_HPP
