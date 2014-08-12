/*!
@file
Defines `boost::hana::Traversable::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_LIST_MCD_HPP
#define BOOST_HANA_TRAVERSABLE_LIST_MCD_HPP

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/traversable/traverse_mcd.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct Traversable::list_mcd : Traversable::traverse_mcd {
        template <typename A, typename F, typename Xs>
        static constexpr auto traverse_impl(F f, Xs xs) {
            auto curried_cons = [](auto x) {
                return [=](auto xs) { return cons(x, xs); };
            };
            return foldr(xs, lift<A>(nil<T>), [=](auto x, auto ys) {
                return ap(fmap(curried_cons, f(x)), ys);
            });
        }
    };

    //! `Traversable` instance for `List` instances.
    //!
    //! ### Example
    //! @snippet example/list/traversable/traverse.cpp main
    //!
    //! ### Example
    //! @snippet example/list/traversable/sequence.cpp main
    template <typename T>
    struct Traversable::instance<T, when<is_a<List, T>()>>
        : Traversable::list_mcd<T>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_LIST_MCD_HPP
