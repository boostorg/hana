/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::IntegerList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGER_LIST_ITERABLE_HPP
#define BOOST_HANA_INTEGER_LIST_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/integer_list/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/mcd.hpp>


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
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGER_LIST_ITERABLE_HPP
