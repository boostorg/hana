/*!
@file
Defines the instance of `boost::hana::Orderable` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_ORDERABLE_HPP
#define BOOST_HANA_FOREIGN_ORDERABLE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/orderable/less_mcd.hpp>
#include <boost/hana/orderable/orderable.hpp>


namespace boost { namespace hana {
    //! Instance of `Orderable` for foreign but `LessThanComparable` objects.
    //!
    //! Any two foreign objects whose types can be compared using `operator<`
    //! are automatically instances of `Orderable` by using that comparison.
    template <typename T, typename U>
    struct Orderable::instance<T, U,
        when<is_valid<decltype((void)(*(T*)0 < *(U*)0))>>
    >
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) < detail::std::forward<Y>(y);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_ORDERABLE_HPP
