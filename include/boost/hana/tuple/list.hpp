/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_LIST_HPP
#define BOOST_HANA_TUPLE_LIST_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple/tuple.hpp>

// Mcd
#include <boost/hana/tuple/monad.hpp>
#include <boost/hana/tuple/foldable.hpp>
#include <boost/hana/tuple/iterable.hpp>


namespace boost { namespace hana {
    //! Instance of `List` for the `Tuple` data type.
    //!
    //! @todo
    //! Use perfect forwarding everywhere possible.
    template <>
    struct List::instance<Tuple> : List::mcd<Tuple> {
        static BOOST_HANA_CONSTEXPR_LAMBDA decltype(auto) nil_impl() {
            return tuple();
        }

        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return detail::std::forward<Xs>(xs).storage(
                [x(detail::std::forward<X>(x))](auto&& ...xs) -> decltype(auto) {
                    return tuple(
                        detail::std::move(x),
                        detail::std::forward<decltype(xs)>(xs)...
                    );
                }
            );
        }

        template <typename ...Xs>
        static constexpr decltype(auto) make_impl(Xs&& ...xs) {
            return tuple(detail::std::forward<Xs>(xs)...);
        }

        template <typename Xs, typename Ys>
        static constexpr decltype(auto) concat_impl(Xs&& xs, Ys&& ys) {
            return detail::std::forward<Xs>(xs).storage(
                [ys(detail::std::forward<Ys>(ys))](auto&& ...xs) -> decltype(auto) {
                    return detail::std::move(ys).storage(
                        //! @todo Initialize the capture with perfect
                        //! forwarding once that's supported by the language.
                        [=](auto&& ...ys) -> decltype(auto) {
                            return tuple(
                                detail::std::move(xs)...,
                                detail::std::forward<decltype(ys)>(ys)...
                            );
                        }
                    );
                }
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) init_impl(Xs&& xs) {
            return unpack(range(size_t<0>, minus(length(xs), size_t<1>)),
                on(tuple, [&xs](auto index) -> decltype(auto) {
                    return at(index, detail::std::forward<Xs>(xs));
                })
            );
        }

        template <typename Xs, typename X>
        static constexpr decltype(auto) snoc_impl(Xs&& xs, X&& x) {
            return detail::std::forward<Xs>(xs).storage(
                [x(detail::std::forward<X>(x))](auto&& ...xs) -> decltype(auto) {
                    return tuple(
                        detail::std::forward<decltype(xs)>(xs)...,
                        detail::std::move(x)
                    );
                }
            );
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) take_impl(N n, Xs&& xs) {
            return unpack(range(size_t<0>, min(n, length(xs))),
                on(tuple, [&xs](auto index) -> decltype(auto) {
                    return at(index, detail::std::forward<Xs>(xs));
                })
            );
        }

        template <typename F, typename ...Xss>
        static constexpr auto zip_with_impl(F f, Xss ...tuples) {
            auto go = [=](auto index, auto ...nothing) {
                return always(f)(nothing...)(at(index, tuples)...);
            };
            auto zip_length = minimum(tuple(length(tuples)...));
            return unpack(range(size_t<0>, zip_length),
                on(tuple, go)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_LIST_HPP
