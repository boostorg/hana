/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/tuple.hpp>

// instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/traversable.hpp>

#include <utility>


struct Tree;

template <typename X, typename Subforest>
struct node_type {
    struct hana { using datatype = Tree; };
    X value;
    Subforest subforest;
};

auto forest = boost::hana::make_tuple;

auto node = [](auto x, auto subforest) {
    return node_type<decltype(x), decltype(subforest)>{x, subforest};
};

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Tree, Tree> {
        template <typename N1, typename N2>
        static constexpr decltype(auto) apply(N1&& n1, N2&& n2) {
            return and_(
                equal(std::forward<N1>(n1).value, std::forward<N2>(n2).value),
                equal(std::forward<N1>(n1).subforest, std::forward<N2>(n2).subforest)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<Tree> {
        template <typename N, typename F>
        static constexpr decltype(auto) apply(N&& n, F f) {
            auto g = [=](auto&& subtree) -> decltype(auto) {
                return transform(std::forward<decltype(subtree)>(subtree), f);
            };
            return node(
                f(std::forward<N>(n).value),
                transform(std::forward<N>(n).subforest, g)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Tree> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return node(std::forward<X>(x), forest()); }
    };

    template <>
    struct ap_impl<Tree> {
        template <typename F, typename X>
        static constexpr decltype(auto) apply(F&& f, X&& x) {
            return node(
                f.value(x.value),
                concat(
                    transform(x.subforest, partial(flip(transform), f.value)),
                    transform(f.subforest, partial(flip(ap), x))
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Tree> {
        template <typename N>
        static constexpr decltype(auto) apply(N&& n) {
            return node(
                std::forward<N>(n).value.value,
                concat(
                    std::forward<N>(n).value.subforest,
                    transform(std::forward<N>(n).subforest, flatten)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct foldl_impl<Tree> {
        template <typename N, typename S, typename F>
        static constexpr decltype(auto) apply(N&& n, S&& s, F f) {
            return foldl(
                std::forward<N>(n).subforest,
                f(std::forward<S>(s), std::forward<N>(n).value),
                [=](auto&& state, auto&& subtree) -> decltype(auto) {
                    return foldl(
                        std::forward<decltype(subtree)>(subtree),
                        std::forward<decltype(state)>(state),
                        f
                    );
                }
            );
        }
    };

    template <>
    struct foldr_impl<Tree> {
        template <typename N, typename S, typename F>
        static constexpr decltype(auto) apply(N&& n, S&& s, F f) {
            return f(
                std::forward<N>(n).value,
                foldr(std::forward<N>(n).subforest, std::forward<S>(s),
                    [=](auto&& subtree, auto&& state) -> decltype(auto) {
                        return foldr(
                            std::forward<decltype(subtree)>(subtree),
                            std::forward<decltype(state)>(state),
                            f
                        );
                    }
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct traverse_impl<Tree> {
        template <typename A, typename N, typename F>
        static constexpr decltype(auto) apply(N&& n, F&& f) {
            return hana::ap(
                hana::transform(f(std::forward<N>(n).value), curry<2>(node)),
                traverse<A>(
                    std::forward<N>(n).subforest,
                    hana::partial(hana::flip(traverse<A>), f)
                )
            );
        }
    };
}}

int main() { }
