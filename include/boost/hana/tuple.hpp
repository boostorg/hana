/*!
@file
Defines the `boost::hana::Tuple` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_HPP
#define BOOST_HANA_TUPLE_HPP

#include <boost/hana/fwd/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/variadic/at.hpp>
#include <boost/hana/detail/variadic/drop_into.hpp>
#include <boost/hana/detail/variadic/take.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/range.hpp>

// instances
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<Tuple> : Foldable::unpack_mcd {
        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return detail::std::forward<Xs>(xs)
                    .storage(detail::std::forward<F>(f));
        }
    };

    template <>
    struct Functor::instance<Tuple> : Functor::fmap_mcd {
        template <typename Xs, typename F>
        static constexpr decltype(auto) fmap_impl(Xs&& xs, F f) {
            return detail::std::forward<Xs>(xs).storage(
                [f(detail::std::move(f))](auto&& ...xs) -> decltype(auto) {
                    return tuple(f(detail::std::forward<decltype(xs)>(xs))...);
                }
            );
        }
    };

    //! Instance of `Iterable` for `Tuple`s.
    //!
    //! `Tuple` is an `Iterable` in the most obvious way. The head of a
    //! non-empty tuple corresponds to its first element. The tail of a
    //! non-empty tuple is a tuple containing all the elements in the same
    //! order, except the head. Finally, a tuple is empty if and only if
    //! it has no elements in it.
    //!
    //! ### Example
    //! @snippet example/list/iterable.cpp main
    template <>
    struct Iterable::instance<Tuple> : Iterable::mcd {
        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return detail::std::forward<Xs>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return id(detail::std::forward<decltype(x)>(x));
                }
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            return detail::std::forward<Xs>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return tuple(detail::std::forward<decltype(rest)>(rest)...);
                }
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) is_empty_impl(Xs&& xs) {
            return detail::std::forward<Xs>(xs).storage(
                [](auto const& ...xs) -> decltype(auto) {
                    return bool_<sizeof...(xs) == 0>;
                }
            );
        }

        template <typename Index, typename Xs>
        static constexpr decltype(auto) at_impl(Index n, Xs&& xs) {
            return detail::std::forward<Xs>(xs).storage(
                detail::variadic::at<value(n)>
            );
        }

        template <typename Index, typename Xs>
        static constexpr decltype(auto) drop_impl(Index n, Xs&& xs) {
            auto m = min(n, length(xs));
            return detail::std::forward<Xs>(xs).storage(
                detail::variadic::drop_into<value(m)>(tuple)
            );
        }
    };

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
            return unpack(range(size_t<0>, pred(length(xs))),
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
            auto m = min(n, length(xs));
            return detail::std::forward<Xs>(xs).storage(
                detail::variadic::take<value(m)>
            )(tuple);
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

#endif // !BOOST_HANA_TUPLE_HPP
