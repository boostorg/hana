/*!
@file
Defines `boost::hana::Searchable::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_LIST_MCD_HPP
#define BOOST_HANA_SEARCHABLE_LIST_MCD_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/searchable/mcd.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `List`
    //!
    //! @todo
    //! Use perfect forwarding once bug
    //! http://llvm.org/bugs/show_bug.cgi?id=20619
    //! is fixed.
    struct Searchable::list_mcd : Searchable::mcd {
        template <typename Xs, typename Pred>
        static constexpr auto find_impl(Xs xs, Pred pred) {
            auto e = drop_until(xs, pred);
            return eval_if(is_empty(e),
                [](auto) { return nothing; },
                [=](auto _) { return just(_(head)(e)); }
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto any_impl(Xs xs, Pred pred) {
            return eval_if(is_empty(xs),
                [](auto _) { return false_; },
                [=](auto _) {
                    return eval_if(pred(_(head)(xs)),
                        [=](auto _) { return true_; },
                        [=](auto _) { return any_impl(_(tail)(xs), pred); }
                    );
                }
            );
        }
    };

    //! Instance of `Searchable` for `List`s.
    //!
    //! A `List` can be searched by doing a linear search in the elements,
    //! with the keys and values being both the elements in the list.
    //!
    //! @todo
    //! Technically, this can be implemented in `Iterable`. Should it?
    //!
    //! ### Example
    //! @snippet example/list/searchable.cpp find
    template <typename T>
    struct Searchable::instance<T, when<is_a<List, T>()>>
        : Searchable::list_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_LIST_MCD_HPP
