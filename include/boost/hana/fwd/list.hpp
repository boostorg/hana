/*!
@file
Forward declares `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LIST_HPP
#define BOOST_HANA_FWD_LIST_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! General purpose index-based sequence.
    //!
    //!
    //!
    //!
    //! `make` for `List`s
    //! ------------------
    //! Create a `List` with the given elements in it.
    //!
    //! @tparam L
    //! The data type representing the `List` to create. This can be any
    //! instance of the `List` type class.
    //!
    //! @param xs...
    //! The elements to put in the constructed list. The elements will appear
    //! in the resulting list in the same order as passed to `make`.
    //!
    //! ### Example
    //! @snippet example/list.cpp make
    //!
    //! ### Benchmarks
    //! @image html benchmark/list/make.ctime.png
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `Monad`, `Iterable`, `Foldable`, `cons`, and `nil`
    //! @todo
    //!
    //!
    //! Provided methods and concepts
    //! -----------------------------
    //! 1. `make`
    //! @todo
    //!
    //! 2. `Comparable`
    //! @todo
    //!
    //!
    //!
    //! @todo
    //! - How to implement iterate and repeat?
    //! - Implement the following methods:
    //!     - `intercalate`, `transpose`, `subsequences`
    //!     - `split_at`, `break`, `inits`, `tails`
    //! - Consider implementing the following methods:
    //!     - `nub_by`, `nub`, `delete_by`, `insert`
    //!     - `set_difference_by`, `set_union_by`, `set_intersection_by`
    //! - Since we can benchmark the isomorphic instances, put the benchmarks
    //!   in the documentation.
    struct List { };


}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LIST_HPP
