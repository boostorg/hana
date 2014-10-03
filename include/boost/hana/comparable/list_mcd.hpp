/*!
@file
Defines `boost::hana::Comparable::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_LIST_MCD_HPP
#define BOOST_HANA_COMPARABLE_LIST_MCD_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `List`
    struct Comparable::list_mcd
        : Comparable::equal_mcd
    {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) equal_impl(Xs const& xs, Ys const& ys) {
            return eval_if(or_(is_empty(xs), is_empty(ys)),
                [&xs, &ys](auto _) -> decltype(auto) {
                    return and_(_(is_empty)(xs), _(is_empty)(ys));
                },
                [&xs, &ys](auto _) -> decltype(auto) {
                    return and_(equal(_(head)(xs), _(head)(ys)),
                                equal_impl(_(tail)(xs), _(tail)(ys)));
                }
            );
        }
    };

    //! Instance of `Comparable` for instances of `List`.
    //!
    //! Two `List`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //!
    //! ### Example
    //! @snippet example/list/comparable.cpp main
    template <typename T, typename U>
    struct Comparable::instance<T, U, when<
        is_a<List, T>() && is_a<List, U>()
    >>
        : Comparable::list_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_LIST_MCD_HPP

#include <boost/hana/comparable/default.hpp> //! @todo remove this include
