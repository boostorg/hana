/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_COMPARABLE_HPP
#define BOOST_HANA_TYPE_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/type/type.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Type`s.
    //!
    //! Two `Type`s are equal if and only if they represent the same C++ type.
    //! Hence, equality is equivalent to the `std::is_same` type trait.
    //!
    //! @snippet example/type/comparable.cpp main
    template <>
    struct Comparable::instance<Type, Type> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto equal_impl(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(T, T)
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_COMPARABLE_HPP
