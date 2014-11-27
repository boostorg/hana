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
#include <boost/hana/integral.hpp>

// instances
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


namespace boost { namespace hana {
    //! Instance of `Iterable` for `IntegerList`s.
    //!
    //! The head of `integer_list<T, x, xs...>` is `integral<T, x>`, its tail
    //! is `integer_list<T, xs...>` and an integer list is empty if and only
    //! if it contains no integers at all.
    template <>
    struct Iterable::instance<IntegerList> : Iterable::mcd {
        template <typename T, T x, T ...xs>
        static constexpr auto head_impl(ilist_detail::integer_list<T, x, xs...>) {
            return integral<T, x>;
        }

        template <typename T, T x, T ...xs>
        static constexpr auto tail_impl(ilist_detail::integer_list<T, x, xs...>) {
            return integer_list<T, xs...>;
        }

        template <typename T, T ...xs>
        static constexpr auto is_empty_impl(ilist_detail::integer_list<T, xs...>) {
            return bool_<sizeof...(xs) == 0>;
        }
    };

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
