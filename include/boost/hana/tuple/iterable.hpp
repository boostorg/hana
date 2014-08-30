/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_ITERABLE_HPP
#define BOOST_HANA_TUPLE_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/variadic/at.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/tuple/tuple.hpp>


namespace boost { namespace hana {
    //! @details
    //! `Tuple` is an `Iterable` in the most obvious way. The head of a
    //! non-empty tuple corresponds to its first element. The tail of a
    //! non-empty tuple is a tuple containing all the elements in the same
    //! order, except the head. Finally, a tuple is empty if and only if
    //! it has no elements in it.
    //!
    //! ### Example
    //! @snippet example/list/iterable.cpp main
    template <>
    struct Iterable::instance<Tuple> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return x;
            });
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return tuple(rest...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            return xs.storage([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }

        template <typename Index, typename Xs>
        static constexpr auto at_impl(Index n, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return detail::variadic::at<n()>(xs...);
            });
        }
    };

}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_ITERABLE_HPP
