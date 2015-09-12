/*!
@file
Defines `boost::hana::LambdaTuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP
#define BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP

#include <boost/hana.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/detail/variadic/at.hpp>
#include <boost/hana/detail/variadic/drop_into.hpp>
#include <boost/hana/detail/variadic/take.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/concept/sequence.hpp>

#include <utility>


namespace boost { namespace hana { namespace sandbox {
    struct LambdaTuple { };

    template <typename Storage>
    struct lambda_tuple_t {
        explicit constexpr lambda_tuple_t(Storage&& s)
            : storage(std::move(s))
        { }

        using hana_tag = LambdaTuple;
        Storage storage;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto lambda_tuple = [](auto ...xs) -> decltype(auto) {
        auto storage = [=](auto f) -> decltype(auto) { return f(xs...); };
        return lambda_tuple_t<decltype(storage)>{std::move(storage)};
    };
}}} // end namespace boost::hana::sandbox

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return static_cast<Xs&&>(xs)
                    .storage(static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F f) {
            return static_cast<Xs&&>(xs).storage(
                [f(std::move(f))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(f(static_cast<decltype(xs)&&>(xs))...);
                }
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct front_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return id(static_cast<decltype(x)&&>(x));
                }
            );
        }
    };

    template <>
    struct tail_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto&& x, auto&& ...rest) -> decltype(auto) {
                    return sandbox::lambda_tuple(static_cast<decltype(rest)&&>(rest)...);
                }
            );
        }
    };

    template <>
    struct is_empty_impl<sandbox::LambdaTuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return static_cast<Xs&&>(xs).storage(
                [](auto const& ...xs) -> decltype(auto) {
                    return _bool<sizeof...(xs) == 0>{};
                }
            );
        }
    };

    template <>
    struct at_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename Index>
        static constexpr decltype(auto) apply(Xs&& xs, Index const&) {
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::at<Index::value>
            );
        }
    };

    template <>
    struct drop_front_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const& n) {
            auto m = min(n, length(xs));
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::drop_into<hana::value(m)>(sandbox::lambda_tuple)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return static_cast<Xs&&>(xs).storage(
                [ys(static_cast<Ys&&>(ys))](auto&& ...xs) -> decltype(auto) {
                    return std::move(ys).storage(
                        // We can't initialize the capture with perfect
                        // forwarding since that's not supported by the
                        // language.
                        [=](auto&& ...ys) -> decltype(auto) {
                            return sandbox::lambda_tuple(
                                std::move(xs)...,
                                static_cast<decltype(ys)&&>(ys)...
                            );
                        }
                    );
                }
            );
        }
    };

    template <>
    struct prepend_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return static_cast<Xs&&>(xs).storage(
                [x(static_cast<X&&>(x))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(
                        std::move(x),
                        static_cast<decltype(xs)&&>(xs)...
                    );
                }
            );
        }
    };

    template <>
    struct append_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return static_cast<Xs&&>(xs).storage(
                [x(static_cast<X&&>(x))](auto&& ...xs) -> decltype(auto) {
                    return sandbox::lambda_tuple(
                        static_cast<decltype(xs)&&>(xs)...,
                        std::move(x)
                    );
                }
            );
        }
    };

    template <>
    struct empty_impl<sandbox::LambdaTuple> {
        static BOOST_HANA_CONSTEXPR_LAMBDA decltype(auto) apply() {
            return sandbox::lambda_tuple();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct Sequence<sandbox::LambdaTuple> {
        static constexpr bool value = true;
    };

    template <>
    struct take_impl<sandbox::LambdaTuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const& n) {
            auto m = min(n, length(xs));
            return static_cast<Xs&&>(xs).storage(
                detail::variadic::take<hana::value(m)>
            )(sandbox::lambda_tuple);
        }
    };

    template <>
    struct zip_shortest_with_impl<sandbox::LambdaTuple> {
        template <typename F, typename ...Xss>
        static constexpr auto apply(F f, Xss ...tuples) {
            auto go = [=](auto index, auto ...nothing) {
                return always(f)(nothing...)(at(tuples, index)...);
            };
            auto zip_length = minimum(sandbox::lambda_tuple(length(tuples)...));
            return unpack(range(size_c<0>, zip_length),
                on(sandbox::lambda_tuple, go)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<sandbox::LambdaTuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return sandbox::lambda_tuple(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SANDBOX_LAMBDA_TUPLE_HPP
