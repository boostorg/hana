/*!
@file
Defines the instance of `boost::hana::Searchable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_SEARCHABLE_HPP
#define BOOST_HANA_STRING_SEARCHABLE_HPP

#include <boost/hana/string/iterable.hpp>
#include <boost/hana/searchable/list_mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Searchable` for `String`s.
    //!
    //! For the purpose of being searched, `String`s are basically equivalent
    //! to a tuple of `IntegralConstant`s of underlying type `char`.
    //!
    //! ### Example
    //! @snippet example/string/searchable.cpp main
    //!
    //! @todo
    //! Don't use the `list_mcd`, or make it an `iterable_mcd`.
    template <>
    struct Searchable::instance<String>
        : Searchable::list_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_SEARCHABLE_HPP
