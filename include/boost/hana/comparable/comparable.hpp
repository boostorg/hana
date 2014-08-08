/*!
@file
Forward declares `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_COMPARABLE_HPP

#include <boost/hana/comparable/detail/comparable_fwd.hpp>

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/integral_fwd.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `equal`
    struct Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return not_(equal(x, y)); }
    };

    //! @details
    //! Objects of different data types that do not define a `Comparable`
    //! instance are implicitly `Comparable` by letting any two such objects
    //! always compare unequal. However, objects of the same data type are
    //! not implicitly `Comparable`.
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

    //! Instance of `Comparable` for objects with `EqualityComparable` data
    //! types.
    //!
    //! Any two objects whose data types can be compared using `operator==`
    //! are automatically instances of `Comparable` by using that comparison.
    template <typename X, typename Y>
    struct Comparable::instance<X, Y, when<is_valid<decltype((void)(*(X*)0 == *(Y*)0))>>>
        : Comparable::equal_mcd
    {
        static constexpr auto equal_impl(X x, Y y)
        { return x == y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_COMPARABLE_HPP
