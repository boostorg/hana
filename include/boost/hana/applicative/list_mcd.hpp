/*!
@file
Defines `boost::hana::Applicative::list_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_LIST_MCD_HPP
#define BOOST_HANA_APPLICATIVE_LIST_MCD_HPP

#include <boost/hana/applicative/applicative.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/monad/monad.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `Functor` and `List`
    template <typename T>
    struct Applicative::list_mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return cons(detail::std::forward<X>(x), nil<T>); }

        template <typename Fs, typename Xs>
        static constexpr decltype(auto) ap_impl(Fs&& fs, Xs&& xs) {
            return bind(
                detail::std::forward<Fs>(fs),
                [xs(detail::std::forward<Xs>(xs))](auto&& f) -> decltype(auto) {
                    return fmap(xs, detail::std::forward<decltype(f)>(f));
                }
            );
        }
    };

    //! `Applicative` instance for instances of the `List` type class.
    //!
    //! A value can be lifted into a singleton list with `lift`. `ap(fs, xs)`
    //! applies each function in the list `fs` to each value in the list `xs`,
    //! and returns a list containing all the results.
    //!
    //! ### Example
    //! @snippet example/list/applicative.cpp main
    template <typename T>
    struct Applicative::instance<T, when<is_a<List, T>()>>
        : Applicative::list_mcd<T>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_LIST_MCD_HPP
