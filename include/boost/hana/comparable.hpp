/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/fwd/comparable.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator==(X&& x, Y&& y) {
            return equal(detail::std::forward<X>(x),
                         detail::std::forward<Y>(y));
        }

        //! Equivalent to `not_equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator!=(X&& x, Y&& y) {
            return not_equal(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }
    }

    //! Minimal complete definition : `equal`
    struct Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) not_equal_impl(X&& x, Y&& y) {
            return not_(equal(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }
    };

    //! Minimal complete definition : `not_equal`
    struct Comparable::not_equal_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            return not_(not_equal(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }
    };
}} // end namespace boost::hana


#include <boost/hana/bool.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/disable.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    template <typename T, typename U, typename>
    struct Comparable::default_instance
        : Comparable::default_instance<T, U, when<true>>
    { };

    //! Default instance for the `Comparable` type class.
    //!
    //! Objects of different data types that do not define a `Comparable`
    //! instance are implicitly `Comparable` by letting any two such objects
    //! always compare unequal. However, objects of the same data type are
    //! not implicitly `Comparable`, and trying to compare two objects of
    //! the same data type that do not define a `Comparable` instance will
    //! result in a compile-time error.
    template <typename T, typename U, bool condition>
    struct Comparable::default_instance<T, U, when<condition>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X const&, Y const&)
        { return false_; }
    };

    template <typename T, typename U>
    struct Comparable::default_instance<T, U, when_valid<common_t<T, U>>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return equal(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };

    template <typename T>
    struct Comparable::default_instance<T, T> : disable {
        // We let it fail if T is not comparable with itself.
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
