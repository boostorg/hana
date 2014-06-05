/*!
@file
Defines `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/at_index/best.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/detail/left_folds/variadic.hpp>
#include <boost/hana/detail/left_folds/variadic_meta.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    /*!
    @datatype{List}
    @{
    General purpose compile-time heterogeneous sequence.

    @instantiates{Iterable, Functor, Foldable, Monad, Comparable}

    @todo
    - How to implement iterate and repeat?
    - Document the list-specific functions.
    - Document how the type classes are instantiated.
     */
    struct List { };

    //! @}

    namespace operators {
        template <typename Storage>
        struct HetList {
            using hana_datatype = List;
            Storage into;
        };

        template <typename ...Xs>
        struct TypeList {
            using hana_datatype = List;

            struct {
                template <typename F>
                constexpr auto operator()(F f) const
                { return f(type<Xs>...); }
            } into;
        };
    }

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        auto into = [=](auto f) { return f(xs...); };
        return operators::HetList<decltype(into)>{into};
    };

    template <typename ...Xs>
    constexpr operators::TypeList<Xs...> list_t{};

    template <typename T, T ...xs>
    BOOST_HANA_CONSTEXPR_LAMBDA auto list_c = list(integral<T, xs>...);

    template <>
    struct Iterable<List> : defaults<Iterable> {
        // TypeList
        template <typename X, typename ...Xs>
        static constexpr auto head_impl(operators::TypeList<X, Xs...>)
        { return type<X>; }

        template <typename X, typename ...Xs>
        static constexpr auto tail_impl(operators::TypeList<X, Xs...>)
        { return list_t<Xs...>; }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(operators::TypeList<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }


        // HetList
        template <typename Storage>
        static constexpr auto head_impl(operators::HetList<Storage> xs) {
            return xs.into([](auto x, ...) {
                return x;
            });
        }

        template <typename Storage>
        static constexpr auto tail_impl(operators::HetList<Storage> xs) {
            return xs.into([](auto, auto ...xs) {
                return list(xs...);
            });
        }

        template <typename Storage>
        static constexpr auto is_empty_impl(operators::HetList<Storage> xs) {
            return xs.into([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }

        template <typename Index, typename Storage>
        static constexpr auto at_impl(Index n, operators::HetList<Storage> xs) {
            return xs.into([=](auto ...xs) {
                return detail::at_index::best(n, xs...);
            });
        }
    };

    template <>
    struct Functor<List> : defaults<Functor> {
        template <typename F, typename Storage>
        static constexpr auto fmap_impl(F f, operators::HetList<Storage> xs) {
            return xs.into([=](auto ...xs) { return list(f(xs)...); });
        }

        template <typename F, typename ...Xs>
        static constexpr auto fmap_impl(F f, operators::TypeList<Xs...>)
        { return list(f(type<Xs>)...); }

        template <template <typename ...> class F, typename ...Xs>
        static constexpr
        auto fmap_impl(type_detail::Lift<F>, operators::TypeList<Xs...>)
        { return list_t<F<Xs>...>; }
    };

    namespace list_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto concat2 = [](auto xs, auto ys) {
            return xs.into([=](auto ...xs) {
                return ys.into([=](auto ...ys) {
                    return list(xs..., ys...);
                });
            });
        };
    }

    template <>
    struct Monad<List> : defaults<Monad> {
        template <typename X>
        static constexpr auto unit_impl(X x)
        { return list(x); }

        template <typename Xxs>
        static constexpr auto join_impl(Xxs xxs)
        { return foldl(list_detail::concat2, list(), xxs); }
    };

    template <>
    struct Foldable<List> : detail::foldable_from_iterable {
        template <typename F, typename State, typename Storage>
        static constexpr auto
        foldl_impl(F f, State s, operators::HetList<Storage> xs) {
            return xs.into([=](auto ...xs) {
                return detail::left_folds::variadic(f, s, xs...);
            });
        }

        template <typename F, typename State, typename ...Xs>
        static constexpr auto
        foldl_impl(F f, State s, operators::TypeList<Xs...>) {
            return detail::left_folds::variadic(f, s, type<Xs>...);
        }

        template <template <typename ...> class F, typename State, typename ...Xs>
        static constexpr auto
        foldl_impl(type_detail::Lift<F>, Type<State>, operators::TypeList<Xs...>) {
            return type<detail::left_folds::variadic_meta<F, State, Xs...>>;
        }

        template <typename F, typename Xs>
        static constexpr auto unpack_impl(F f, Xs xs)
        { return xs.into(f); }

        template <typename ...Xs>
        static constexpr auto length_impl(operators::TypeList<Xs...>)
        { return size_t<sizeof...(Xs)>; }

        template <typename Storage>
        static constexpr auto length_impl(operators::HetList<Storage> xs) {
            return xs.into([](auto ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }
    };

    template <>
    struct Comparable<List, List>
        : detail::comparable_from_iterable
    { };


    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...lists) {
        return unpack(
            on(list, [=](auto index) { return f(at(index, lists)...); }),
            range(size_t<0>, minimum(list(length(lists)...)))
        );
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto xs) {
        return unpack(
            on(list, [=](auto index) { return at(index, xs); }),
            range(size_t<0>, length(xs) - size_t<1>)
        );
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto ...lists) {
        return zip_with(list, lists...);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return xs.into([=](auto ...xs) { return list(x, xs...); });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return xs.into([=](auto ...xs) { return list(xs..., x); });
    };


    //! @todo Move `min` in a proper type class.
    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto n, auto xs) {
        auto min = [](auto a, auto b) { return if_(a < b, a, b); };
        return unpack(
            on(list, [=](auto index) { return at(index, xs); }),
            range(size_t<0>, min(n, length(xs)))
        );
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = fix(
        [](auto take_while, auto pred, auto xs) {
            auto go = [=](auto xs) {
                return if_(pred(head(xs)),
                    [=](auto xs) {
                        return cons(head(xs), take_while(pred, tail(xs)));
                    },
                    always(list())
                )(xs);
            };
            return if_(is_empty(xs), id, go)(xs);
        }
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return foldl(flip(cons), list(), xs);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto pred, auto xs) {
        auto go = [=](auto x, auto xs) {
            return if_(pred(x), cons, always(xs))(x, xs);
        };
        return foldr(go, list(), xs);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto ...lists) {
        return foldl(list_detail::concat2, list(), list(lists...));
    };

    //! @todo Use a more efficient implementation.
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto pred, auto xs) {
        return list(filter(pred, xs), filter(compose(!_, pred), xs));
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = fix(
        [](auto sort_by, auto pred, auto xs) {
            return if_(is_empty(xs),
                always(xs),
                [=](auto xs) {
                    return if_(is_empty(tail(xs)),
                        always(xs),
                        [=](auto xs) {
                            auto pivot = head(xs);
                            auto rest = tail(xs);
                            auto parts = partition(partial(pred, pivot), rest);
                            return concat(
                                sort_by(pred, at(int_<1>, parts)),
                                cons(pivot, sort_by(pred, at(int_<0>, parts)))
                            );
                        }
                    )(xs);
                }
            )(xs);
        }
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto xs) {
        return sort_by(_ < _, xs);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
