/*!
@file
Defines `boost::hana::Functor::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_LIST_MCD_HPP
#define BOOST_HANA_FUNCTOR_LIST_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/list/list.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct Functor::list_mcd : Functor::fmap_mcd {
        template <typename Xs, typename F>
        static constexpr auto fmap_impl(Xs xs, F f) {
            return foldr(xs, nil<T>, [=](auto x, auto xs) {
                return cons(f(x), xs);
            });
        }
    };

    //! `Functor` instance for `List`s.
    //!
    //! `List`s implement `fmap` as the mapping of a function over each
    //! element of the list, which is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example 1
    //! @snippet example/list/functor/fmap.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/functor.cpp fmap
    template <typename T>
    struct Functor::instance<T, when<is_a<List, T>()>>
        : Functor::list_mcd<T>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_LIST_MCD_HPP
