/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_FOLDABLE_MINIMAL_FOLDABLE_HPP
#define BOOST_HANA_TEST_FOLDABLE_MINIMAL_FOLDABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable.hpp>


struct MinimalFoldable;

template <typename Storage>
struct _foldable {
    Storage storage;
    using hana_datatype = MinimalFoldable;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = [](auto ...xs) {
    auto storage = [=](auto f) { return f(xs...); };
    return _foldable<decltype(storage)>{storage};
};

namespace boost { namespace hana {
    template <>
    struct Foldable::instance<MinimalFoldable> : Foldable::lazy_foldr_mcd {
        struct helper {
            template <typename F, typename S, typename X, typename ...Xs>
            constexpr auto operator()(F f, S s, X x, Xs ...xs) const {
                return f(
                    [=] { return x; },
                    [=](auto ...nothing) {
                        static_assert(sizeof...(nothing) == 0, "");
                        return lazy_foldr(f, s, foldable(xs..., nothing...));
                    }
                );
            }

            template <typename F, typename S>
            constexpr auto operator()(F f, S s) const
            { return s; }
        };

        template <typename F, typename S, typename Xs>
        static constexpr auto lazy_foldr_impl(F f, S s, Xs xs) {
            return xs.storage([=](auto ...xs) { return helper{}(f, s, xs...); });
        }
    };
}}

#endif // !BOOST_HANA_TEST_FOLDABLE_MINIMAL_FOLDABLE_HPP
