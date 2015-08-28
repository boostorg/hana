/*!
@file
Forward declares `boost::hana::BasicTuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_BASIC_TUPLE_HPP
#define BOOST_HANA_FWD_BASIC_TUPLE_HPP

#include <boost/hana/fwd/core/make.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Stripped down version of `hana::tuple`.
    //!
    //! Whereas `hana::tuple` aims to provide an interface somewhat close to a
    //! `std::tuple`, `BasicTuple` provides the strict minimum required to
    //! implement a closure with maximum compile-time efficiency.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! For now, `BasicTuple` only models the `Foldable` concept. More will
    //! be added in the future, and `BasicTuple` will eventually model
    //! everything that `hana::tuple` models.
    struct BasicTuple { };

    template <typename ...Xs>
    struct basic_tuple;

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Function object for creating a `BasicTuple`.
    //! @relates BasicTuple
    //!
    //! Given zero or more objects `xs...`, `make<BasicTuple>` returns a new
    //! `BasicTuple` containing those objects. The elements are held by value
    //! inside the resulting tuple, and they are hence copied or moved in.
    //! This is analogous to `std::make_tuple` for creating `BasicTuple`s.
    //!
    //!
    //! Example
    //! -------
    //! @include example/basic_tuple/make.cpp
    template <>
    constexpr auto make<BasicTuple> = [](auto&& ...xs)
        -> basic_tuple<std::decay_t<decltype(xs)>...>
    {
        return {forwarded(xs)...};
    };
#endif

    //! Alias to `make<BasicTuple>`; provided for convenience.
    //! @relates BasicTuple
    constexpr auto make_basic_tuple = make<BasicTuple>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_BASIC_TUPLE_HPP
