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
    General purpose compile-time heterogeneous sequence.

    @instantiates{Iterable, Functor, Foldable, Monad, Comparable}

    @todo
    - It might be possible to optimize the implementation of homogeneous lists
      using an array.
    - Is it desirable to have different ways of creating lists, or should we
      in fact provide `type_list`, `homogeneous_list`, etc...?
    - How to implement iterate and repeat?
    - Document how the type classes are instantiated.
     */
    struct List { };

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

    //! Creates a `List` containing `xs...`.
    //! @relates List
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        auto into = [=](auto f) { return f(xs...); };
        return operators::HetList<decltype(into)>{into};
    };

    //! Creates a `List` of types.
    //! @relates List
    //!
    //! This is functionally equivalent to `list(type<xs>...)`.
    //!
    //! @note
    //! `list_t` may be more efficient than its `list` counterpart because
    //! of optimizations. When possible, `list_t` should be preferred.
    //!
    //! ### Example
    //! @snippet example/list/list_t.cpp main
    //!
    //! @bug
    //! The types will be instantiated when ADL triggers.
    template <typename ...xs>
    constexpr operators::TypeList<xs...> list_t{};

    //! Creates a `List` of `Integral`s.
    //! @relates List
    //!
    //! This is functionally equivalent to `list(integral<T, xs>...)`.
    //!
    //! @note
    //! `list_c` may be more efficient than its `list` counterpart because
    //! of optimizations. When possible, `list_c` should be preferred.
    //!
    //! ### Example
    //! @snippet example/list/list_c.cpp main
    //!
    //! @todo
    //! Actually provide optimizations.
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
            return xs.into([](auto x, auto ...rest) {
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
        auto fmap_impl(type_detail::Template<F>, operators::TypeList<Xs...>)
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
        foldl_impl(type_detail::Template<F>, operators::_type<State>, operators::TypeList<Xs...>) {
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

    /*!
    Zip one list or more with a given function.
    @relates List

    Specifically, returns a list whose i-th element is `f(s1[i], ..., sn[i])`,
    where `sk[i]` denotes the i-th element of the k-th list passed as an
    argument. The returned list stops when the shortest input sequence is
    exhausted.

    In other words, `zip_with(f, s1, ..., sn)` is a list of the form
    @code
        f(s1[0], ..., sn[0])
        f(s1[1], ..., sn[1])
        ...
        f(s1[k], ..., sn[k])
    @endcode
    where `k` is the length of the shortest list.

    ### Example
    @snippet example/list/zip_with.cpp main

    @todo
    - Consider allowing only two lists and achieving the variadic behavior
    in some other way. This would make it possible to automatically curry
    `zip_with`. It might be possible to achieve the variadic behavior with
    e.g. Applicative Functors? If we decide to keep the variadic behavior,
    then we probably want to support the 0 list case. How should `zip_with`
    behave in that case?
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...lists) {
        static_assert(sizeof...(lists) >= 1,
            "zip_with requires at least one list");
        return unpack(
            on(list, [=](auto index) { return f(at(index, lists)...); }),
            range(size_t<0>, minimum(list(length(lists)...)))
        );
    };

    //! Zip one list or more.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/zip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto ...lists) {
        return zip_with(list, lists...);
    };

    //! Removes the last element of a non-empty list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/init.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto xs) {
        return unpack(
            on(list, [=](auto index) { return at(index, xs); }),
            range(size_t<0>, length(xs) - size_t<1>)
        );
    };

    //! Prepend an element to the head of a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/cons.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return xs.into([=](auto ...xs) { return list(x, xs...); });
    };

    //! Append an element to the end of a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/snoc.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return xs.into([=](auto ...xs) { return list(xs..., x); });
    };


    //! Return the first `n` elements of a list.
    //! @relates List
    //!
    //! `n` must be a non-negative `Integral` representing the number of
    //! elements to keep. If `n` is greater than the length of the list,
    //! the whole list is returned.
    //!
    //! ### Example
    //! @snippet example/list/take.cpp main
    //!
    //! @todo Move `min` in a proper type class.
    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto n, auto xs) {
        auto min = [](auto a, auto b) { return if_(a < b, a, b); };
        return unpack(
            on(list, [=](auto index) { return at(index, xs); }),
            range(size_t<0>, min(n, length(xs)))
        );
    };

    //! Take elements while the `predicate` is satisfied.
    //! @relates List
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements satisfy the given predicate.
    //!
    //! @todo
    //! How to specify that a predicate must be compile-time?
    //!
    //! ### Example
    //! @snippet example/list/take_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = [](auto predicate, auto xs) {
        auto acc = [=](auto x, auto xs) {
            return if_(predicate(x()),
                [=](auto xs) { return cons(x(), xs()); },
                always(list())
            )(xs);
        };
        return lazy_foldr(acc, list(), xs);
    };

    //! Take elements until the `predicate` is satisfied.
    //! @relates List
    //!
    //! This is equivalent to `take_while` with a negated predicate.
    //!
    //! ### Example
    //! @snippet example/list/take_until.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_until = [](auto predicate, auto xs) {
        return take_while([=](auto x) { return !predicate(x); }, xs);
    };

    //! Reverse a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/reverse.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return foldl(flip(cons), list(), xs);
    };

    //! Return a list containing only the elements satisfying the `predicate`.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/filter.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto predicate, auto xs) {
        auto go = [=](auto x, auto xs) {
            return if_(predicate(x), cons, always(xs))(x, xs);
        };
        return foldr(go, list(), xs);
    };

    //! Concatenate zero or more lists together.
    //! @relates List
    //!
    //! With 0 arguments, returns an empty list. With 1 argument, returns
    //! the list itself.
    //!
    //! ### Example
    //! @snippet example/list/concat.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto ...lists) {
        return foldl(list_detail::concat2, list(), list(lists...));
    };

    //! Partition a list based on a `predicate`.
    //! @relates List
    //!
    //! Specifically, returns a list whose first element is a list of the
    //! elements satisfying the predicate, and whose second element is a list
    //! of the elements that do not satisfy the predicate.
    //!
    //! ### Fusion example
    //! @snippet example/list/partition.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/partition.cpp mpl
    //!
    //! @todo Use a more efficient implementation.
    //! @todo Express the fact that we need the predicate to be compile-time.
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto predicate, auto xs) {
        auto not_pred = [=](auto x) { return !predicate(x); };
        return list(filter(predicate, xs), filter(not_pred, xs));
    };

    //! Sort a list based on the given `predicate`.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/sort_by.cpp main
    //!
    //! @todo Document the properties of the `predicate`.
    //! @todo Document the properties of the sort. Is it stable?
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = fix(
        [](auto sort_by, auto predicate, auto xs) {
            return if_(is_empty(xs),
                always(xs),
                [=](auto xs) {
                    return if_(is_empty(tail(xs)),
                        always(xs),
                        [=](auto xs) {
                            auto pivot = head(xs);
                            auto rest = tail(xs);
                            auto parts = partition(partial(predicate, pivot), rest);
                            return concat(
                                sort_by(predicate, at(int_<1>, parts)),
                                cons(pivot, sort_by(predicate, at(int_<0>, parts)))
                            );
                        }
                    )(xs);
                }
            )(xs);
        }
    );

    //! Sort a list based on the `<` partial ordering.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/sort.cpp main
    //!
    //! @todo Use a real type class method for Orderables when we get one.
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto xs) {
        return sort_by(_ < _, xs);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
