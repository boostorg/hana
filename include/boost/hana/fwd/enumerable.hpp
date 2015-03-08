/*!
@file
Forward declares `boost::hana::Enumerable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ENUMERABLE_HPP
#define BOOST_HANA_FWD_ENUMERABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
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
    //! Minimal complete definition
    //! ---------------------------
    //! `succ` and `pred` satisfying the above laws
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For non-boolean arithmetic data types\n
    //! A data type `T` is arithmetic if `std::is_arithmetic<T>::%value` is
    //! true. For a non-boolean arithmetic data type `T`, a model of
    //! `Enumerable` is automatically defined by setting
    //! @code
    //!     succ(x) = ++x
    //!     pred(x) = --x
    //! @endcode
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Successor_function
    //! [2]: http://en.wikipedia.org/wiki/Countable_set
    struct Enumerable { };

    //! Returns the successor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/enumerable.cpp succ
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto succ = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct succ_impl;

    struct _succ {
        template <typename E>
        constexpr decltype(auto) operator()(E&& num) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(models<Enumerable, typename datatype<E>::type>{},
        "hana::succ(n) requires n to be Enumerable");
#endif

            return succ_impl<
                typename datatype<E>::type
            >::apply(detail::std::forward<E>(num));
        }
    };

    constexpr _succ succ{};
#endif

    //! Returns the predecessor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/enumerable.cpp pred
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pred = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct pred_impl;

    struct _pred {
        template <typename E>
        constexpr decltype(auto) operator()(E&& num) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(models<Enumerable, typename datatype<E>::type>{},
        "hana::pred(n) requires n to be Enumerable");
#endif

            return pred_impl<
                typename datatype<E>::type
            >::apply(detail::std::forward<E>(num));
        }
    };

    constexpr _pred pred{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ENUMERABLE_HPP
