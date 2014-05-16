/*!
 * @file
 * Defines `boost::hana::Type`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename T>
    struct Type { };

    template <typename T>
    constexpr Type<T> type{};

    template <typename T, typename U>
    constexpr Bool<false> operator==(Type<T>, Type<U>) { return {}; }
    template <typename T>
    constexpr Bool<true> operator==(Type<T>, Type<T>) { return {}; }

    template <typename T, typename U>
    constexpr auto operator!=(Type<T> t, Type<U> u) -> decltype(!(t == u))
    { return !(t == u); }
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
