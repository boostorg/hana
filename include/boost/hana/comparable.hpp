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


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T, typename U>
    struct Comparable;

    constexpr struct _equal {
        template <typename T, typename U>
        constexpr auto operator()(T t, U u) const
        { return Comparable<T, U>::equal_impl(t, u); }
    } equal{};

    constexpr struct _not_equal {
        template <typename T, typename U>
        constexpr auto operator()(T t, U u) const
        { return Comparable<T, U>::not_equal_impl(t, u); }
    } not_equal{};

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
