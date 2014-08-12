/*!
@file
Defines the default instance for `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_DEFAULT_HPP
#define BOOST_HANA_COMPARABLE_DEFAULT_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/disable.hpp>


namespace boost { namespace hana {
    //! Default instance for the `Comparable` type class.
    //!
    //! Objects of different data types that do not define a `Comparable`
    //! instance are implicitly `Comparable` by letting any two such objects
    //! always compare unequal. However, objects of the same data type are
    //! not implicitly `Comparable`, and trying to compare two objects of
    //! the same data type that do not define a `Comparable` instance will
    //! result in a compile-time error.
    template <typename T, typename U>
    struct Comparable::default_instance : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X, Y)
        { return false_; }
    };

    template <typename T>
    struct Comparable::default_instance<T, T> : disable {
        // We let it fail if T is not comparable with itself.
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_DEFAULT_HPP
