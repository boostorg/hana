/*!
@file
Forward declares `boost::hana::IntegerList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGER_LIST_HPP
#define BOOST_HANA_FWD_INTEGER_LIST_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! `List` containing `Integral`s of the same underlying type only.
    //!
    //! ## Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`, `Iterable`,
    //! `Foldable`, `List`, `Traversable` and `Searchable`
    //!
    //! @note
    //! `IntegerList` may be more efficient than `Tuple` because of
    //! optimizations. When possible, it should be preferred.
    struct IntegerList { struct hana_enabled_operators : Comparable, Monad { }; };

    namespace ilist_detail {
        template <typename T, T ...xs>
        struct integer_list : operators::enable_adl {
            using hana_datatype = IntegerList;
        };
    }

    //! Creates a list containing the given `Integral`s.
    //! @relates IntegerList
    //!
    //! This is functionally equivalent to `tuple(integral<T, xs>...)`.
    //!
    //! ### Example
    //! @snippet example/integer_list/integer_list.cpp main
    template <typename T, T ...xs>
    constexpr ilist_detail::integer_list<T, xs...> integer_list{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGER_LIST_HPP
