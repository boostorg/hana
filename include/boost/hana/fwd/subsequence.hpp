/*!
@file
Forward declares `boost::hana::subsequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SUBSEQUENCE_HPP
#define BOOST_HANA_FWD_SUBSEQUENCE_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns the elements at the given indices of a sequence.
    //! @relates Sequence
    //!
    //! Given a (finite) and compile-time `Foldable` containing indices,
    //! `subsequence` returns a new `Sequence` of the elements of the
    //! original sequence that appear at those indices. The indices must
    //! be `Constant`s of an unsigned integral type. In other words,
    //! @code
    //!     subsequence([x1, ..., xn], [i1, ..., ik]) == [xi1, ..., xik]
    //! @endcode
    //!
    //! In particular, note that indices do not have to be ordered or
    //! sequential in any particular way, and they may contain duplicates.
    //!
    //!
    //! @param xs
    //! The sequence from which a subsequence is extracted.
    //!
    //! @param indices
    //! A compile-time `Foldable` holding `Constant`s of an unsigned integral
    //! type, and whose linearization represents the indices of the elements
    //! in the returned sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp subsequence
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto subsequence = [](auto&& xs, auto&& indices) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct subsequence_impl : subsequence_impl<S, when<true>> { };

    struct subsequence_t {
        template <typename Xs, typename Indices>
        constexpr auto operator()(Xs&& xs, Indices&& indices) const;
    };

    static constexpr subsequence_t subsequence{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SUBSEQUENCE_HPP
