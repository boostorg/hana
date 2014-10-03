/*!
@file
Defines `boost::hana::Comparable::record_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_RECORD_MCD_HPP
#define BOOST_HANA_COMPARABLE_RECORD_MCD_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/record/record.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    template <typename R>
    struct Comparable::record_mcd : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X const& x, Y const& y) {
            return all(members<R>, [&x, &y](auto&& member) -> decltype(auto) {
                auto accessor = second(detail::std::forward<decltype(member)>(member));
                return equal(accessor(x), accessor(y));
            });
        }
    };

    //! Two `Records` of the same data type `R` are equal if and only if
    //! all their members are equal. The members are compared in the
    //! same order as they appear in `members<R>`.
    template <typename R>
    struct Comparable::instance<R, R, when<is_a<Record, R>()>>
        : Comparable::record_mcd<R>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_RECORD_MCD_HPP
