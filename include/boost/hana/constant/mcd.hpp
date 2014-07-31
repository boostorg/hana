/*!
@file
Defines `boost::hana::Constant::mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_MCD_HPP
#define BOOST_HANA_CONSTANT_MCD_HPP

#include <boost/hana/constant/constant.hpp>

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/integral_fwd.hpp>


namespace boost { namespace hana {
    // Note: Constant::mcd is actually defined in constant/constant.hpp
    // to help with circular dependencies.

    //! Instance of `Comparable` for `Constant`s.
    //!
    //! Two `Constant`s are equal if and only if their underlying values
    //! can be compared and are equal.
    //!
    //! ### Example
    //! @snippet example/constant/comparable.cpp main
    template <typename C1, typename C2>
    struct Comparable::instance<C1, C2, when<
        is_a<Constant, C1>() && is_a<Constant, C2>()
    >> : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
            -> decltype(bool_<value(x) == value(y)>)
        { return bool_<value(x) == value(y)>; }

        static constexpr auto equal_impl(...)
        { return false_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_MCD_HPP

#include <boost/hana/bool.hpp>
