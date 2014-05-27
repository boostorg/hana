/*!
 * @file
 * Defines `boost::hana::Comparable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T, typename U>
    struct Comparable {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x == y; }

        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return x != y; }
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               equal_impl(x, y);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               not_equal_impl(x, y);
    };

    namespace operators {
        template <typename T, typename U>
        constexpr auto operator==(T t, U u)
        { return equal(t, u); }

        template <typename T, typename U>
        constexpr auto operator!=(T t, U u)
        { return not_equal(t, u); }
    }

    template <>
    struct defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return !not_equal(t, u); }

        template <typename T, typename U>
        static constexpr auto not_equal_impl(T t, U u)
        { return !equal(t, u); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
