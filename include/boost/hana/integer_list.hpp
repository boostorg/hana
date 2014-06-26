/*!
@file
Defines `boost::hana::IntegerList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGER_LIST_HPP
#define BOOST_HANA_INTEGER_LIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! `List` containing `Integral`s of the same underlying type only.
    //!
    //! @note
    //! `IntegerList` may be more efficient than `List` because of
    //! optimizations. When possible, it should be preferred.
    //!
    //! @todo
    //! Actually provide optimizations.
    struct IntegerList { };

    namespace operators {
        template <typename T, T ...xs>
        struct integer_list {
            using hana_datatype = IntegerList;
        };
    }

    //! Creates a list containing the given `Integral`s.
    //! @relates IntegerList
    //!
    //! This is functionally equivalent to `list(integral<T, xs>...)`.
    //!
    //! ### Example
    //! @snippet example/integer_list/integer_list.cpp main
    template <typename T, T ...xs>
    constexpr operators::integer_list<T, xs...> integer_list{};

    //! @details
    //! Generic instance for `Iterable`s.
    template <>
    struct Foldable::instance<IntegerList> : Iterable::FoldableInstance {

    };

    //! @details
    //! The head of `integer_list<T, x, xs...>` is `integral<T, x>`, its tail
    //! is `integer_list<T, xs...>` and an integer list is empty if and only
    //! if it contains no integers at all.
    template <>
    struct Iterable::instance<IntegerList> : Iterable::mcd {
        template <typename T, T x, T ...xs>
        static constexpr auto head_impl(operators::integer_list<T, x, xs...>) {
            return integral<T, x>;
        }

        template <typename T, T x, T ...xs>
        static constexpr auto tail_impl(operators::integer_list<T, x, xs...>) {
            return integer_list<T, xs...>;
        }

        template <typename T, T ...xs>
        static constexpr auto is_empty_impl(operators::integer_list<T, xs...>) {
            return bool_<sizeof...(xs) == 0>;
        }
    };

    //! @details
    //! An `Integral` can be prepended to an `IntegerList` by using `cons`;
    //! however the `cons`ed value has to be convertible to the type of the
    //! integers in the list. An empty `IntegerList` can be created with
    //! `nil<IntegerList>`; the type of the `Integral`s it contains will be
    //! set when an element is added to the list.
    template <>
    struct List::instance<IntegerList> : List::mcd<IntegerList> {
        template <typename X, typename T, T ...xs>
        static constexpr auto cons_impl(X x, operators::integer_list<T, xs...>) {
            return integer_list<T, static_cast<T>(x), xs...>;
        }

        template <typename X>
        static constexpr auto cons_impl(X x, operators::integer_list<void>) {
            return integer_list<decltype(x()), x()>;
        }

        static constexpr auto nil_impl() {
            return integer_list<void>;
        }
    };

    //! @details
    //! Generic instance for `Iterable`s.
    template <>
    struct Comparable::instance<IntegerList, IntegerList>
        : Iterable::ComparableInstance
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGER_LIST_HPP
