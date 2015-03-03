/*!
@file
Forward declares `boost::hana::Pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PAIR_HPP
#define BOOST_HANA_FWD_PAIR_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/create.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Generic container of two elements.
    //!
    //! A `Pair` is essentially equivalent to a `std::pair`, which can also
    //! be seen as a 2-element tuple. `Pair`s are useful in some contexts
    //! where a tuple would be too much, like returning two elements from a
    //! function.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two pairs `(x, y)` and `(x', y')` are equal if and only if both
    //! `x == x'` and `y == y'`.
    //! @snippet example/pair.cpp comparable
    //!
    //! 2. `Orderable` (operators provided)\n
    //! Pairs are ordered as-if they were 2-element tuples, using a
    //! lexicographical ordering.
    //! @snippet example/pair.cpp orderable
    //!
    //! 3. `Foldable`\n
    //! Folding a `Pair` is equivalent to folding a 2-element tuple. In other
    //! words:
    //! @code
    //!     foldl(pair(x, y), s, f) == f(f(s, x), y)
    //!     foldr(pair(x, y), s, f) == f(x, f(y, s))
    //! @endcode
    //! Example:
    //! @snippet example/pair.cpp foldable
    //!
    //! 4. `Product`\n
    //! The model of `Product` is the simplest one possible; the first element
    //! of a pair `(x, y)` is `x`, and its second element is `y`.
    //! @snippet example/pair.cpp product
    struct Pair { };

    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair.cpp pair
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pair = [](auto&& first, auto&& second) {
        return unspecified-type;
    };
#else
    template <typename First, typename Second, typename = operators::adl>
    struct _pair {
        First first;
        Second second;
        struct hana { using datatype = Pair; };
    };

    constexpr detail::create<_pair> pair{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PAIR_HPP
