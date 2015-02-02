/*!
@file
Defines `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/fwd/list.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/product.hpp>

// provided instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    template <typename L>
    struct make_impl<L, when<is_a<List, L>{}>> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return detail::variadic::foldr(cons, nil<S>(),
                detail::std::forward<Xs>(xs)...
            );
        }
    };


    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, when<is_a<List, L>{} && is_a<Foldable, T>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return foldr(detail::std::forward<Xs>(xs), nil<S>(), cons); }
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
    struct lift_impl<T, when<is_a<List, T>{}>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return cons(detail::std::forward<X>(x), nil<T>()); }
    };

    template <typename T>
    struct ap_impl<T, when<is_a<List, T>{}>> {
        template <typename Fs, typename Xs>
        static constexpr decltype(auto) apply(Fs&& fs, Xs&& xs) {
            return bind(
                detail::std::forward<Fs>(fs),
                partial(transform, detail::std::forward<Xs>(xs))
            );
        }
    };

    //! `Functor` instance for `List`s.
    //!
    //! `List`s implement `transform` as the mapping of a function over each
    //! element of the list, which is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example
    //! @snippet example/list/functor.cpp transform
    template <typename T>
    struct transform_impl<T, when<is_a<List, T>{}>> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return foldr(
                detail::std::forward<Xs>(xs),
                nil<T>(),
                compose(cons, detail::std::forward<F>(f))
            );
        }
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
    struct flatten_impl<T, when<is_a<List, T>{}>> {
        template <typename Xss>
        static constexpr decltype(auto) apply(Xss&& xss) {
            return foldl(detail::std::forward<Xss>(xss), nil<T>(), concat);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
