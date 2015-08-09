/*!
@file
Forward declares `boost::hana::Pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PAIR_HPP
#define BOOST_HANA_FWD_PAIR_HPP

#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Tag representing a generic container of two elements.
    //!
    //! A `Pair` is essentially equivalent to a `std::pair`, which can also
    //! be seen as a 2-element tuple. `Pair`s are useful in some contexts
    //! where a tuple would be too much, like returning two elements from a
    //! function.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable`\n
    //! Two pairs `(x, y)` and `(x', y')` are equal if and only if both
    //! `x == x'` and `y == y'`.
    //! @include example/pair/comparable.cpp
    //!
    //! 2. `Orderable`\n
    //! Pairs are ordered as-if they were 2-element tuples, using a
    //! lexicographical ordering.
    //! @include example/pair/orderable.cpp
    //!
    //! 3. `Foldable`\n
    //! Folding a `Pair` is equivalent to folding a 2-element tuple. In other
    //! words:
    //! @code
    //!     fold_left(make_pair(x, y), s, f) == f(f(s, x), y)
    //!     fold_right(make_pair(x, y), s, f) == f(x, f(y, s))
    //! @endcode
    //! Example:
    //! @include example/pair/foldable.cpp
    //!
    //! 4. `Product`\n
    //! The model of `Product` is the simplest one possible; the first element
    //! of a pair `(x, y)` is `x`, and its second element is `y`.
    //! @include example/pair/product.cpp
    struct Pair { };

    template <typename First, typename Second>
    struct pair;

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    //!
    //!
    //! Example
    //! -------
    //! @include example/pair/make.cpp
    template <>
    constexpr auto make<Pair> = [](auto&& first, auto&& second)
        -> pair<std::decay_t<decltype(first)>, std::decay_t<decltype(second)>>
    {
        return {forwarded(first), forwarded(second)};
    };
#endif

    //! Alias to `make<Pair>`; provided for convenience.
    //! @relates Pair
    //!
    //! Example
    //! -------
    //! @include example/pair/make.cpp
    constexpr auto make_pair = make<Pair>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PAIR_HPP
