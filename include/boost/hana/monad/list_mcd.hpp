/*!
@file
Defines `boost::hana::Monad::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_LIST_MCD_HPP
#define BOOST_HANA_MONAD_LIST_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct Monad::list_mcd : Monad::flatten_mcd<T> {
        template <typename Xss>
        static constexpr auto flatten_impl(Xss xss)
        { return foldl(xss, nil<T>, concat); }
    };

    //! `Monad` instance for instances of the `List` type class.
    //!
    //! A function returning a list of results can be mapped over all the
    //! elements of a list and have all the results concatenated using `bind`.
    //! Also, a list of lists can be flattened one level with `flatten`.
    //!
    //! ### Example
    //! @snippet example/list/monad.cpp main
    template <typename T>
    struct Monad::instance<T, when<is_a<List, T>()>>
        : Monad::list_mcd<T>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_LIST_MCD_HPP
