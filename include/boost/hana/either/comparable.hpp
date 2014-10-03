/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Either`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_COMPARABLE_HPP
#define BOOST_HANA_EITHER_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/either/either.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Either`s.
    //!
    //! Two `Either`s are equal if and only if they both contain left values
    //! or they both contain right values and those values are equal.
    //!
    //! ### Example
    //! @snippet example/either.cpp comparable
    template <>
    struct Comparable::instance<Either, Either> : Comparable::equal_mcd {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) equal_impl(E<T> const& x, E<U> const& y) {
            return equal(x.value, y.value);
        }

        template <template <typename ...> class E1,
                  template <typename ...> class E2, typename T, typename U>
        static constexpr auto equal_impl(E1<T> const&, E2<U> const&)
        { return false_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_COMPARABLE_HPP
