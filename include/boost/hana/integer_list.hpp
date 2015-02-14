/*!
@file
Defines `boost::hana::IntegerList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGER_LIST_HPP
#define BOOST_HANA_INTEGER_LIST_HPP

#include <boost/hana/fwd/integer_list.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/integral_constant.hpp>


#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<IntegerList> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(ilist_detail::integer_list<T, x, xs...>) {
            return integral_constant<T, x>;
        }
    };

    template <>
    struct tail_impl<IntegerList> {
        template <typename T, T x, T ...xs>
        static constexpr auto apply(ilist_detail::integer_list<T, x, xs...>) {
            return integer_list<T, xs...>;
        }
    };

    template <>
    struct is_empty_impl<IntegerList> {
        template <typename T, T ...xs>
        static constexpr auto apply(ilist_detail::integer_list<T, xs...>) {
            return bool_<sizeof...(xs) == 0>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<IntegerList>
        : Iterable::find_impl<IntegerList>
    { };

    template <>
    struct any_impl<IntegerList>
        : Iterable::any_impl<IntegerList>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct foldl_impl<IntegerList>
        : Iterable::foldl_impl<IntegerList>
    { };

    template <>
    struct foldr_impl<IntegerList>
        : Iterable::foldr_impl<IntegerList>
    { };

    //! Instance of `List` for `IntegerList`s.
    //!
    //! An `Integral` can be prepended to an `IntegerList` by using `cons`;
    //! however the `cons`ed value has to be convertible to the type of the
    //! integers in the list. An empty `IntegerList` can be created with
    //! `nil<IntegerList>()`; the type of the `Integral`s it contains will
    //! be set when an element is added to the list.
    template <>
    struct List::instance<IntegerList> : List::mcd<IntegerList> {
        template <typename X, typename T, T ...xs>
        static constexpr auto cons_impl(X x, ilist_detail::integer_list<T, xs...>) {
            return integer_list<T, static_cast<T>(x), xs...>;
        }

        template <typename X>
        static constexpr auto cons_impl(X x, ilist_detail::integer_list<void>) {
            return integer_list<decltype(x()), x()>;
        }

        static constexpr auto nil_impl() {
            return integer_list<void>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGER_LIST_HPP
