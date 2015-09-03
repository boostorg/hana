/*!
@file
Forward declares `boost::hana::basic_tuple`.

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
    //! `std::tuple`, `basic_tuple` provides the strict minimum required to
    //! implement a closure with maximum compile-time efficiency.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! For now, `basic_tuple` only models the `Foldable` concept. More will
    //! be added in the future, and `basic_tuple` will eventually model
    //! everything that `hana::tuple` models.
    template <typename ...Xs>
    struct basic_tuple;

    //! Tag representing `hana::basic_tuple`.
    //! @relates hana::basic_tuple
    struct basic_tuple_tag { };

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Function object for creating a `basic_tuple`.
    //! @relates hana::basic_tuple
    //!
    //! Given zero or more objects `xs...`, `make<basic_tuple_tag>` returns a
    //! new `basic_tuple` containing those objects. The elements are held by
    //! value inside the resulting tuple, and they are hence copied or moved
    //! in. This is analogous to `std::make_tuple` for creating `basic_tuple`s.
    //!
    //!
    //! Example
    //! -------
    //! @include example/basic_tuple/make.cpp
    template <>
    constexpr auto make<basic_tuple_tag> = [](auto&& ...xs) {
        return basic_tuple<std::decay_t<decltype(xs)>...>{forwarded(xs)...};
    };
#endif

    //! Alias to `make<basic_tuple_tag>`; provided for convenience.
    //! @relates hana::basic_tuple
    //!
    //!
    //! Example
    //! -------
    //! @include example/basic_tuple/make.cpp
    constexpr auto make_basic_tuple = make<basic_tuple_tag>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_BASIC_TUPLE_HPP
