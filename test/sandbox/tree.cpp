/*
@copyright Louis Dionne 2014
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
#include <boost/hana/list.hpp>
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

auto forest = boost::hana::tuple;

auto node = [](auto x, auto subforest) {
    return node_type<decltype(x), decltype(subforest)>{x, subforest};
};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Tree, Tree> : Comparable::equal_mcd {
        template <typename N1, typename N2>
        static constexpr decltype(auto) equal_impl(N1&& n1, N2&& n2) {
            return and_(
                equal(std::forward<N1>(n1).value, std::forward<N2>(n2).value),
                equal(std::forward<N1>(n1).subforest, std::forward<N2>(n2).subforest)
            );
        }
    };

    template <>
    struct Functor::instance<Tree> : Functor::fmap_mcd {
        template <typename N, typename F>
        static constexpr decltype(auto) fmap_impl(N&& n, F f) {
            auto g = [=](auto&& subtree) -> decltype(auto) {
                return fmap(std::forward<decltype(subtree)>(subtree), f);
            };
            return node(
                f(std::forward<N>(n).value),
                fmap(std::forward<N>(n).subforest, g)
            );
        }
    };

    template <>
    struct Foldable::instance<Tree> : Foldable::folds_mcd {
        template <typename N, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(N&& n, S&& s, F f) {
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

        template <typename N, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(N&& n, S&& s, F f) {
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

    template <>
    struct Applicative::instance<Tree> : Applicative::mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return node(std::forward<X>(x), forest()); }

        template <typename F, typename X>
        static constexpr decltype(auto) ap_impl(F&& f, X&& x) {
            return node(
                f.value(x.value),
                concat(
                    fmap(x.subforest, partial(flip(fmap), f.value)),
                    fmap(f.subforest, partial(flip(ap), x))
                )
            );
        }
    };

    template <>
    struct Monad::instance<Tree> : Monad::flatten_mcd<Tree> {
        template <typename N>
        static constexpr decltype(auto) flatten_impl(N&& n) {
            return node(
                std::forward<N>(n).value.value,
                concat(
                    std::forward<N>(n).value.subforest,
                    fmap(std::forward<N>(n).subforest, flatten)
                )
            );
        }
    };

    template <>
    struct Traversable::instance<Tree> : Traversable::traverse_mcd {
        template <typename A, typename N, typename F>
        static constexpr decltype(auto) traverse_impl(N&& n, F&& f) {
            return ap(
                fmap(f(std::forward<N>(n).value), curry<2>(node)),
                traverse<A>(
                    std::forward<N>(n).subforest,
                    partial(flip(traverse<A>), f)
                )
            );
        }
    };
}}

int main() { }
