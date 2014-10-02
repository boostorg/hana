/*!
@file
Defines the instance of `boost::hana::Comparable` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_COMPARABLE_HPP
#define BOOST_HANA_FOREIGN_COMPARABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for objects of foreign types.
    //!
    //! Any two foreign objects whose types can be compared using
    //! `operator==` are automatically instances of `Comparable` by
    //! using that comparison.
    template <typename T, typename U>
    struct Comparable::instance<T, U,
        when<is_valid<decltype((void)(*(T*)0 == *(U*)0))>>
    >
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) == detail::std::forward<Y>(y);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_COMPARABLE_HPP
