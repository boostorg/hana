/*!
@file
Defines `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/at_index/best.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/enable_if.hpp>
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
    @ingroup datatypes
    General purpose compile-time heterogeneous sequence.

    @note
    Any `Foldable` can be converted to a `List`.

    --------------------------------------------------------------------------

    ## Instance of

    ### Iterable
    `List` is an `Iterable` in the most obvious way. The head of a non-empty
    list corresponds to its first element. The tail of a non-empty list is
    a list containing all the elements in the same order, except the head.
    Finally, a list is empty if and only if it has no elements in it.
    @snippet example/list/iterable/overview.cpp main

    ### Functor
    `List` implements `fmap` as the mapping of a function over each element
    of the list. This is somewhat equivalent to `std::transform`. Mapping a
    function over an empty list returns an empty list and never applies the
    function.
    #### Example 1
    @snippet example/list/functor/fmap.cpp fusion
    #### Example 2
    @snippet example/list/functor/fmap.cpp mpl

    ### Applicative
    A value can be lifted into a singleton list with `unit<List>`. `ap(fs, xs)`
    applies each function in the list `fs` to each value in the list `xs`, and
    returns a list containing all the results.
    @snippet example/list/applicative/overview.cpp main

    ### Monad
    A function returning a list of results can be mapped over all the elements
    of a list and have all the results concatenated using `bind`. Also, a
    list of lists can be flattened one level with `join`.
    @snippet example/list/monad/overview.cpp main

    ### Foldable
    Generic instance for `Iterable`s.

    ### Comparable
    Generic instance for `Iterable`s.

    --------------------------------------------------------------------------

    @todo
    - Re-enable the foldl and fmap optimization for type lists.
    - It might be possible to optimize the implementation of homogeneous lists
      using an array.
    - Is it desirable to have different ways of creating lists, or should we
      in fact provide `type_list`, `homogeneous_list`, etc...?
    - How to implement iterate and repeat?
    - Check laws for `Applicative`.
     */
    struct List { };

    namespace list_detail {
        template <typename ...xs>
        struct type_container { };

        template <typename ...xs>
        struct hidden_type_container {
            struct type { using contents = type_container<xs...>; };
        };
    }

    namespace operators {
        template <typename Storage>
        struct HetList {
            using hana_datatype = List;
            Storage into;
        };

        template <typename HiddenTypeContainer>
        struct TypeList {
            using hana_datatype = List;

            struct {
                //! @cond
                template <typename F, typename ...Xs>
                static constexpr auto call(F f, list_detail::type_container<Xs...>)
                { return f(type<Xs>...); }

                template <typename F>
                constexpr auto operator()(F f) const
                { return call(f, typename HiddenTypeContainer::contents{}); }
                //! @endcond
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
    //! @todo
    //! We can have efficient membership testing for those.
    template <typename ...xs>
    constexpr operators::TypeList<
        typename list_detail::hidden_type_container<xs...>::type
    > list_t{};

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
    struct Iterable<List> : defaults<Iterable>::with<List> {
        // TypeList
        template <typename X, typename ...Xs>
        static constexpr auto head_helper(list_detail::type_container<X, Xs...>)
        { return type<X>; }
        template <typename HiddenTypeContainer>
        static constexpr auto head_impl(operators::TypeList<HiddenTypeContainer>)
        { return head_helper(typename HiddenTypeContainer::contents{}); }


        template <typename X, typename ...Xs>
        static constexpr auto tail_helper(list_detail::type_container<X, Xs...>)
        { return list_t<Xs...>; }
        template <typename HiddenTypeContainer>
        static constexpr auto tail_impl(operators::TypeList<HiddenTypeContainer>)
        { return tail_helper(typename HiddenTypeContainer::contents{}); }


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

        template <typename Index, typename Storage>
        static constexpr auto at_impl(Index n, operators::HetList<Storage> xs) {
            return xs.into([=](auto ...xs) {
                return detail::at_index::best<n()>(xs...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return length(xs) == size_t<0>; }
    };

    template <>
    struct Functor<List> : defaults<Functor>::with<List> {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs)
        { return xs.into([=](auto ...xs) { return list(f(xs)...); }); }

        // template <template <typename ...> class F, typename ...Xs>
        // static constexpr auto fmap_helper(type_detail::Template<F>, list_detail::type_container<Xs...>)
        // { return list_t<F<Xs>...>; }
        // template <template <typename ...> class F, typename HiddenTypeContainer>
        // static constexpr auto fmap_impl(type_detail::Template<F> f, operators::TypeList<HiddenTypeContainer>)
        // { return fmap_helper(f, typename HiddenTypeContainer::contents{}); }
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
    struct Applicative<List> : defaults<Applicative>::with<List> {
        template <typename X>
        static constexpr auto unit_impl(X x)
        { return list(x); }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };

    template <>
    struct Monad<List> : defaults<Monad>::with<List> {
        template <typename Xss>
        static constexpr auto join_impl(Xss xss)
        { return foldl(list_detail::concat2, list(), xss); }
    };

    template <>
    constexpr bool foldable_from_iterable<List> = true;

    template <>
    struct Foldable<List> : instance<Foldable>::with<List> {
        template <typename F, typename State, typename Xs>
        static constexpr auto
        foldl_impl(F f, State s, Xs xs) {
            return xs.into([=](auto ...xs) {
                return detail::left_folds::variadic(f, s, xs...);
            });
        }

        // template <template <typename ...> class F, typename State, typename ...Xs>
        // static constexpr auto
        // foldl_impl(type_detail::Template<F>, operators::_type<State>, operators::TypeList<Xs...>) {
        //     return type<detail::left_folds::variadic_meta<F, State, Xs...>>;
        // }

        template <typename F, typename Xs>
        static constexpr auto unpack_impl(F f, Xs xs)
        { return xs.into(f); }


        template <typename ...Xs>
        static constexpr auto length_helper(list_detail::type_container<Xs...>)
        { return size_t<sizeof...(Xs)>; }
        template <typename HiddenTypeContainer>
        static constexpr auto length_impl(operators::TypeList<HiddenTypeContainer>)
        { return length_helper(typename HiddenTypeContainer::contents{}); }

        template <typename Storage>
        static constexpr auto length_impl(operators::HetList<Storage> xs) {
            return xs.into([](auto ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }
    };

    template <>
    constexpr bool comparable_from_iterable<List> = true;

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

    //! Prepend an element to the head of a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/cons.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return xs.into([=](auto ...xs) { return list(x, xs...); });
    };

    //! Return a list containing only the elements satisfying the `predicate`.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/filter.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto predicate, auto xs) {
        auto go = [=](auto x, auto xs) {
            return eval_if(predicate(x),
                [=](auto _) { return _(cons)(x, xs); },
                always(xs)
            );
        };
        return foldr(go, list(), xs);
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

    //! Append an element to the end of a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/snoc.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return xs.into([=](auto ...xs) { return list(xs..., x); });
    };

    //! Partition a list based on a `predicate`.
    //! @relates List
    //!
    //! Specifically, returns a list whose first element is a list of the
    //! elements satisfying the predicate, and whose second element is a
    //! list of the elements that do not satisfy the predicate.
    //!
    //! @note
    //! The predicate must return an `Integral`.
    //!
    //! ### Fusion example
    //! @snippet example/list/partition.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/partition.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto predicate, auto xs) {
        auto go = [=](auto parts, auto x) {
            auto yes = head(parts);
            auto no = head(tail(parts));
            return if_(predicate(x),
                list(snoc(yes, x), no),
                list(yes, snoc(no, x))
            );
        };
        return foldl(go, list(list(), list()), xs);
    };

    namespace list_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto insertions = curry<2>(fix(
            [](auto insertions, auto x, auto l) {
                return eval_if(is_empty(l),
                    always(list(list(x))),
                    [=](auto _) {
                        auto y = _(head)(l);
                        auto ys = _(tail)(l);
                        return cons(
                            _(cons)(x, l),
                            fmap(partial(cons, y), insertions(x, ys))
                        );
                    }
                );
            }
        ));
    }

    //! Return a list of all the permutations of the given list.
    //! @relates List
    //!
    //! The permutations are not guaranteed to be in any specific order.
    //!
    //! ### Example
    //! @snippet example/list/permutations.cpp main
    //!
    //! @note
    //! Implementation taken from http://stackoverflow.com/a/2184129/627587.
    //!
    //! @bug
    //! We got a performance problem here. Generating the permutations of
    //! a list of more than 3 elements starts taking a long time (>6s).
    BOOST_HANA_CONSTEXPR_LAMBDA auto permutations = fix(
        [](auto permutations, auto xs) {
            return eval_if(is_empty(xs),
                always(list(list())),
                [=](auto _) {
                    return join(fmap(
                        list_detail::insertions(_(head)(xs)),
                        permutations(_(tail)(xs))
                    ));
                }
            );
        }
    );

    //! Reverse a list.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/reverse.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return foldl(flip(cons), list(), xs);
    };

    //! Sort a list based on the given `predicate`.
    //! @relates List
    //!
    //! The predicate must be a [strict weak ordering](http://en.wikipedia.org/wiki/Strict_weak_ordering#Strict_weak_orderings).
    //! The sort is guaranteed to be stable.
    //!
    //! ### Example
    //! @snippet example/list/sort_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = fix(
        [](auto sort_by, auto predicate, auto xs) {
            return eval_if(is_empty(xs),
                always(xs),
                [=](auto _) {
                    return eval_if(is_empty(_(tail)(xs)),
                        always(xs),
                        [=](auto _) {
                            auto pivot = _(head)(xs);
                            auto rest = _(tail)(xs);
                            auto parts = partition([=](auto x) { return predicate(x, pivot); }, rest);
                            auto smaller = at(int_<0>, parts);
                            auto greater_equal = at(int_<1>, parts);
                            return concat(
                                sort_by(predicate, smaller),
                                cons(pivot, sort_by(predicate, greater_equal))
                            );
                        }
                    );
                }
            );
        }
    );

    //! Sort a list based on the `<` strict weak ordering.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/sort.cpp main
    //!
    //! @todo Use a real type class method for Orderables when we get one.
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto xs) {
        return sort_by(_ < _, xs);
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
    //! elements satisfy the given predicate. The predicate must return
    //! an `Integral`.
    //!
    //! ### Example
    //! @snippet example/list/take_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = [](auto predicate, auto xs) {
        auto acc = [=](auto x, auto xs) {
            return eval_if(predicate(x()),
                [=](auto _) { return cons(x(), _(xs)()); },
                always(list())
            );
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
        return take_while([=](auto x) { return not_(predicate(x)); }, xs);
    };

    /*!
    Zip several lists with a given function.
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

    @note
    When invoked with 0 arguments, returns an empty list.

    ### Example
    @snippet example/list/zip_with.cpp main

    @todo
    - Consider allowing only two lists and achieving the variadic behavior
    in some other way. This would make it possible to automatically curry
    `zip_with`. It might be possible to achieve the variadic behavior with
    e.g. Applicative Functors?
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...lists) {
        auto go = [=](auto index) {
            return always(f)(index)(at(index, lists)...);
        };
        auto zip_length = eval_if(bool_<sizeof...(lists) == 0>,
            always(size_t<0>),
            [=](auto _) { return minimum(_(list)(length(lists)...)); }
        );
        return unpack(on(list, go), range(size_t<0>, zip_length));
    };

    //! Zip one list or more.
    //! @relates List
    //!
    //! ### Example
    //! @snippet example/list/zip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto ...lists) {
        return zip_with(list, lists...);
    };

    template <typename T>
    struct convert<List, T, detail::enable_if_t<instantiates<Foldable, T>>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return foldr(cons, list(), xs); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
