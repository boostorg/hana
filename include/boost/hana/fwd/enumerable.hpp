/*!
@file
Forward declares `boost::hana::Enumerable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ENUMERABLE_HPP
#define BOOST_HANA_FWD_ENUMERABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Represents data types whose values can be enumerated.
    //!
    //! `Enumerable` provides the `succ` and `pred` methods, whose names
    //! come from the [successor][1] and predecessor functions used when
    //! defining the natural numbers with the Peano axioms. Those functions
    //! allow the values of a data type to be enumerated. Note that an
    //! `Enumerable` data type `E` may contain infinitely many values;
    //! `pred` and `succ` can be implemented as long as the infinity is
    //! [countable][2] (in its mathematical sense).
    //!
    //!
    //! Laws
    //! ----
    //! For any `Enumerable x`, the following laws must be satisfied:
    //! @code
    //!     succ(pred(x)) == x
    //!     pred(succ(x)) == x
    //! @endcode
    //!
    //!
    //! Minimal complete definintions
    //! -----------------------------
    //! 1. `succ` and `pred`
    //!
    //! 2. Data type with a valid `operator++` and `operator--`
    //! Any object with a _data type_ that can be incremented using `operator++`
    //! and decremented using `operator--` is `Enumerable` using those
    //! operations for `succ` and `pred` respectively. If the data type of
    //! the object is not the same as its `decltype`, and if the object may
    //! not be incremented or decremented with `operator++` and `operator--`,
    //! then a compile-time error will arise.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Successor_function
    //! [2]: http://en.wikipedia.org/wiki/Countable_set
    struct Enumerable { };

    //! Returns the successor of a value.
    //! @relates Enumerable
    //!
    //! ### Example
    //! @snippet example/enumerable.cpp succ
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto succ = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(succ_impl);

    struct _succ {
        template <typename Num>
        constexpr decltype(auto) operator()(Num&& num) const {
            return dispatch<succ_impl<
                typename datatype<Num>::type
            >>::apply(detail::std::forward<Num>(num));
        }
    };

    constexpr _succ succ{};
#endif

    //! Returns the predecessor of a value.
    //! @relates Enumerable
    //!
    //! ### Example
    //! @snippet example/enumerable.cpp pred
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pred = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(pred_impl);

    struct _pred {
        template <typename Num>
        constexpr decltype(auto) operator()(Num&& num) const {
            return dispatch<pred_impl<
                typename datatype<Num>::type
            >>::apply(detail::std::forward<Num>(num));
        }
    };

    constexpr _pred pred{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ENUMERABLE_HPP
