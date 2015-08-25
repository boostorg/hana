/*!
@file
Forward declares `boost::hana::Enumerable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_ENUMERABLE_HPP
#define BOOST_HANA_FWD_CONCEPT_ENUMERABLE_HPP

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
    //! Minimal complete definition
    //! ---------------------------
    //! `succ` and `pred` satisfying the laws below
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
    //! Concrete models
    //! ---------------
    //! `hana::integral_constant`
    //!
    //!
    //! Free model for non-boolean arithmetic data types
    //! ------------------------------------------------
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
    template <typename E>
    struct Enumerable;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONCEPT_ENUMERABLE_HPP
