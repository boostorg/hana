/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_HPP
#define BOOST_HANA_CONSTANT_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! Represents a compile-time constant.
    //!
    //! @todo
    //! Integrate this with `Integral` and the rest of the library.
    struct Constant { };

    namespace constant_detail {
        template <typename T, T t>
        struct constant : operators::enable {
            using hana_datatype = Constant;
        };
    }

    //! Two `Constant`s are equal if and only if their underlying values can
    //! be compared and are equal.
    //!
    //! ### Example
    //! @snippet example/constant/comparable.cpp main
    template <>
    struct Comparable::instance<Constant, Constant> : Comparable::equal_mcd {
        template <typename X, X x, typename Y, Y y>
        static constexpr auto equal_impl(
            constant_detail::constant<X, x>,
            constant_detail::constant<Y, y>
        ) -> decltype(bool_<x == y>)
        { return bool_<x == y>; }

        static constexpr auto equal_impl(...)
        { return false_; }
    };

    //! Creates a compile-time constant of the given type and holding the
    //! given value.
    //! @relates Constant
    template <typename T, T t>
    constexpr constant_detail::constant<T, t> constant{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP
