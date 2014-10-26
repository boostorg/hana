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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Represents data types whose values can be enumerated.
    //!
    //! `Enumerable` provides the `succ` and `pred` functions, whose names
    //! come from the [successor][1] and predecessor functions used when
    //! defining the natural numbers with the Peano axioms. Those functions
    //! allow the values of a data type to be enumerated. Note that an
    //! `Enumerable` data type `E` may contain infinitely many values;
    //! `pred` and `succ` can be implemented as long as the infinity is
    //! [countable][2] (in its mathematical sense).
    //!
    //! [1]: http://en.wikipedia.org/wiki/Successor_function
    //! [2]: http://en.wikipedia.org/wiki/Countable_set
    struct Enumerable {
        BOOST_HANA_TYPECLASS(Enumerable);
        struct mcd;
        template <typename I>
        struct integral_constant_mcd;
    };

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
    struct _succ {
        template <typename Num>
        constexpr decltype(auto) operator()(Num&& num) const {
            return Enumerable::instance<
                datatype_t<Num>
            >::succ_impl(detail::std::forward<Num>(num));
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
    struct _pred {
        template <typename Num>
        constexpr decltype(auto) operator()(Num&& num) const {
            return Enumerable::instance<
                datatype_t<Num>
            >::pred_impl(detail::std::forward<Num>(num));
        }
    };

    constexpr _pred pred{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ENUMERABLE_HPP
