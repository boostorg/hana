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
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>

#include <type_traits> // for std::is_same


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // The List type class
    //////////////////////////////////////////////////////////////////////////

    BOOST_HANA_TYPECLASS_BOILERPLATE(struct List)

    /*!
    @ingroup typeclasses
    @ingroup datatypes
    General purpose index-based sequence.

    --------------------------------------------------------------------------

    ## Instance of (as a data type)
    `Iterable`, `Functor`, `Applicative`, `Monad`, `Foldable` and `Comparable`

    --------------------------------------------------------------------------

    @todo
    - It might be possible to optimize the implementation of homogeneous lists
      using an array.
    - How to implement iterate and repeat?
    - Check laws for `Applicative`.
    - Get rid of the `<type_traits>` include.
     */
    struct List : typeclass<List> {
        template <typename T>
        struct mcd;
    };

    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename Storage>
        struct list {
            using hana_datatype = List;
            Storage storage;
        };
    }

    //! Creates a `List` containing `xs...`.
    //! @relates List
    //!
    //! The `List` data type is a general purpose compile-time heterogeneous
    //! sequence.
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        auto storage = [=](auto f) { return f(xs...); };
        return operators::list<decltype(storage)>{storage};
    };
    //////////////////////////////////////////////////////////////////////////


    //! Concatenate two lists together.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/concat.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto concat = [](auto xs, auto ys) {
        static_assert(std::is_same<datatype_t<decltype(xs)>, datatype_t<decltype(ys)>>{}, "");
        return List::instance<datatype_t<decltype(xs)>>::concat_impl(xs, ys);
    };

    //! Prepend an element to the head of a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/cons.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto cons = [](auto x, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::cons_impl(x, xs);
    };

    //! Return a list containing only the elements satisfying the `predicate`.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/filter.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto predicate, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::filter_impl(predicate, xs);
    };

    //! Removes the last element of a non-empty list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/init.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::init_impl(xs);
    };

    namespace list_detail {
        template <typename T>
        struct into {
            template <typename ...Xs>
            constexpr auto operator()(Xs ...xs) const
            { return List::instance<T>::into_impl(xs...); }
        };
    }

    //! Creates a `List` with the given elements in it.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/into.cpp main
    template <typename T>
    constexpr list_detail::into<T> into{};

    //! An empty list.
    //! @method{List}
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto nil = List::instance<T>::nil_impl();

    //! Partition a list based on a `predicate`.
    //! @method{List}
    //!
    //! Specifically, returns a `Pair` whose first element is a list of the
    //! elements satisfying the predicate, and whose second element is a
    //! list of the elements that do not satisfy the predicate.
    //!
    //! @note
    //! The predicate must return an `Integral`.
    //!
    //! ### Example 1
    //! @snippet example/integer_list/partition.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/partition.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto predicate, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::partition_impl(predicate, xs);
    };

    //! Return a list of all the permutations of the given list.
    //! @method{List}
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
    BOOST_HANA_CONSTEXPR_LAMBDA auto permutations = [](auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::permutations_impl(xs);
    };

    //! Reverse a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/reverse.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto reverse = [](auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::reverse_impl(xs);
    };

    //! Append an element to the end of a list.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/snoc.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto snoc = [](auto xs, auto x) {
        return List::instance<datatype_t<decltype(xs)>>::snoc_impl(xs, x);
    };

    //! Sort a list based on the `<` strict weak ordering.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/sort.cpp main
    //!
    //! @todo Use a real type class method for Orderables when we get one.
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort = [](auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::sort_impl(xs);
    };

    //! Sort a list based on the given `predicate`.
    //! @method{List}
    //!
    //! The predicate must be a [strict weak ordering](http://en.wikipedia.org/wiki/Strict_weak_ordering#Strict_weak_orderings).
    //! The sort is guaranteed to be stable.
    //!
    //! ### Example
    //! @snippet example/list/sort_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto sort_by = [](auto predicate, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::sort_by_impl(predicate, xs);
    };

    //! Return the first `n` elements of a list.
    //! @method{List}
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
        return List::instance<datatype_t<decltype(xs)>>::take_impl(n, xs);
    };

    //! Take elements until the `predicate` is satisfied.
    //! @method{List}
    //!
    //! This is equivalent to `take_while` with a negated predicate.
    //!
    //! ### Example
    //! @snippet example/integer_list/take_until.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_until = [](auto predicate, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::take_until_impl(predicate, xs);
    };

    //! Take elements while the `predicate` is satisfied.
    //! @method{List}
    //!
    //! Specifically, returns the longest prefix of a list in which all
    //! elements satisfy the given predicate. The predicate must return
    //! an `Integral`.
    //!
    //! ### Example
    //! @snippet example/integer_list/take_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto take_while = [](auto predicate, auto xs) {
        return List::instance<datatype_t<decltype(xs)>>::take_while_impl(predicate, xs);
    };

    namespace list_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto zip() { return list(); }

        template <typename Xs, typename ...Xss>
        constexpr auto zip(Xs xs, Xss ...xss) {
            return List::instance<datatype_t<Xs>>::zip_impl(xs, xss...);
        }

        template <typename F>
        constexpr auto zip_with(F) { return list(); }

        template <typename F, typename Xs, typename ...Xss>
        constexpr auto zip_with(F f, Xs xs, Xss ...xss) {
            return List::instance<datatype_t<Xs>>::zip_with_impl(f, xs, xss...);
        }
    }

    //! Zip zero lists or more.
    //! @method{List}
    //!
    //! ### Example
    //! @snippet example/list/zip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip = [](auto ...lists) {
        return list_detail::zip(lists...);
    };

    /*!
    Zip several lists with a given function.
    @method{List}

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
    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...xss) {
        return list_detail::zip_with(f, xss...);
    };

    //! @details
    //! Requires `Iterable`, `Foldable`, `cons`, and `nil`.
    template <typename T>
    struct List::mcd {
        template <typename Xs, typename Ys>
        static constexpr auto concat_impl(Xs xs, Ys ys)
        { return foldr(cons, ys, xs); }

        template <typename Pred, typename Xs>
        static constexpr auto filter_impl(Pred pred, Xs xs) {
            auto go = [=](auto x, auto xs) {
                return eval_if(pred(x),
                    [=](auto _) { return _(cons)(x, xs); },
                    always(xs)
                );
            };
            return foldr(go, nil<datatype_t<Xs>>, xs);
        }

        template <typename Xs>
        static constexpr auto init_impl(Xs xs) {
            return eval_if(is_empty(tail(xs)),
                always(nil<datatype_t<Xs>>),
                [=](auto _) { return cons(_(head)(xs), init_impl(_(tail)(xs))); }
            );
        }

        template <typename ...Xs>
        static constexpr auto into_impl(Xs ...xs) {
            return foldr(cons, nil<T>, list(xs...));
        }

        template <typename Pred, typename Xs>
        static constexpr auto partition_impl(Pred pred, Xs xs) {
            auto go = [=](auto parts, auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return pair(_(snoc)(first(parts), x), second(parts)); },
                    [=](auto _) { return pair(first(parts), _(snoc)(second(parts), x)); }
                );
            };
            return foldl(go, pair(nil<T>, nil<T>), xs);
        }

    private:
        template <typename X, typename Xs>
        static constexpr auto insertions(X x, Xs l) {
            return eval_if(is_empty(l),
                always(lift<T>(lift<T>(x))),
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

    public:
        template <typename Xs>
        static constexpr auto permutations_impl(Xs xs) {
            return eval_if(is_empty(xs),
                always(lift<T>(nil<T>)),
                [=](auto _) {
                    return join(fmap(
                        [=](auto ys) { return insertions(_(head)(xs), ys); },
                        permutations_impl(_(tail)(xs))
                    ));
                }
            );
        }

        template <typename Xs>
        static constexpr auto reverse_impl(Xs xs) {
            return foldl(flip(cons), nil<T>, xs);
        }

        template <typename Xs, typename X>
        static constexpr auto snoc_impl(Xs xs, X x) {
            return foldr(cons, lift<T>(x), xs);
        }

        template <typename Xs>
        static constexpr auto sort_impl(Xs xs) {
            return sort_by(_ < _, xs);
        }

        template <typename Pred, typename Xs>
        static constexpr auto sort_by_impl(Pred pred, Xs xs) {
            return eval_if(is_empty(xs),
                always(xs),
                [=](auto _) {
                    return eval_if(is_empty(_(tail)(xs)),
                        always(xs),
                        [=](auto _) {
                            auto pivot = _(head)(xs);
                            auto rest = _(tail)(xs);
                            auto parts = partition([=](auto x) { return pred(x, pivot); }, rest);
                            return concat(
                                sort_by_impl(pred, first(parts)),
                                cons(pivot, sort_by_impl(pred, second(parts)))
                            );
                        }
                    );
                }
            );
        }

        template <typename N, typename Xs>
        static constexpr auto take_impl(N n, Xs xs) {
            return eval_if(or_(is_empty(xs), equal(n, int_<0>)),
                always(nil<T>),
                [=](auto _) {
                    return cons(_(head)(xs), take_impl(n - int_<1>, _(tail)(xs)));
                }
            );
        }

        template <typename Pred, typename Xs>
        static constexpr auto take_until_impl(Pred pred, Xs xs) {
            return take_while([=](auto x) { return not_(pred(x)); }, xs);
        }

        template <typename Pred, typename Xs>
        static constexpr auto take_while_impl(Pred pred, Xs xs) {
            auto acc = [=](auto x, auto xs) {
                return eval_if(pred(x()),
                    [=](auto _) { return cons(x(), _(xs)()); },
                    always(nil<T>)
                );
            };
            return lazy_foldr(acc, nil<T>, xs);
        }

        template <typename ...Xss>
        static constexpr auto zip_impl(Xss ...xss)
        { return zip_with(into<T>, xss...); }

        template <typename F, typename ...Xss>
        static constexpr auto zip_with_impl(F f, Xss ...xss) {
            return eval_if(any_of(list(is_empty(xss)...)),
                always(nil<T>),
                [=](auto _) {
                    return cons(
                        f(_(head)(xss)...),
                        zip_with_impl(f, _(tail)(xss)...)
                    );
                }
            );
        }
    };

    //! @details
    //! `List`s implement `fmap` as the mapping of a function over each
    //! element of the list. This is somewhat equivalent to `std::transform`.
    //! Mapping a function over an empty list returns an empty list and never
    //! applies the function.
    //!
    //! ### Example 1
    //! @snippet example/list/functor/fmap.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/functor/fmap.cpp main
    template <typename T>
    struct Functor::instance<T, detail::enable_if_t<instantiates<List, T>>>
        : Functor::fmap_mcd
    {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            return foldr(compose(cons, f), nil<T>, xs);
        }
    };

    //! @details
    //! A value can be lifted into a singleton list with `lift`. `ap(fs, xs)`
    //! applies each function in the list `fs` to each value in the list `xs`,
    //! and returns a list containing all the results.
    //!
    //! ### Example
    //! @snippet example/list/applicative/overview.cpp main
    template <typename T>
    struct Applicative::instance<T, detail::enable_if_t<instantiates<List, T>>>
        : Applicative::mcd
    {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return cons(x, nil<T>); }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };

    //! @details
    //! A function returning a list of results can be mapped over all the
    //! elements of a list and have all the results concatenated using `bind`.
    //! Also, a list of lists can be flattened one level with `join`.
    //!
    //! ### Example
    //! @snippet example/list/monad/overview.cpp main
    template <typename T>
    struct Monad::instance<T, detail::enable_if_t<instantiates<List, T>>>
        : Monad::join_mcd
    {
        template <typename Xss>
        static constexpr auto join_impl(Xss xss)
        { return foldl(concat, nil<T>, xss); }
    };

    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, detail::enable_if_t<
        instantiates<List, L> && instantiates<Foldable, T>
    >> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return foldr(cons, nil<L>, xs); }
    };

    //////////////////////////////////////////////////////////////////////////
    // The List data type
    //////////////////////////////////////////////////////////////////////////
    //! @details
    //! Generic instance for `Iterable`s.
    template <>
    struct Foldable::instance<List> : Iterable::FoldableInstance {
        template <typename F, typename State, typename Xs>
        static constexpr auto foldl_impl(F f, State s, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return detail::left_folds::variadic(f, s, xs...);
            });
        }

        template <typename F, typename Xs>
        static constexpr auto unpack_impl(F f, Xs xs) {
            return xs.storage(f);
        }

        template <typename Xs>
        static constexpr auto length_impl(Xs xs) {
            return xs.storage([](auto ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }
    };

    //! @details
    //! `List` is an `Iterable` in the most obvious way. The head of a
    //! non-empty list corresponds to its first element. The tail of a
    //! non-empty list is a list containing all the elements in the same
    //! order, except the head. Finally, a list is empty if and only if
    //! it has no elements in it.
    //!
    //! ### Example
    //! @snippet example/list/iterable/overview.cpp main
    template <>
    struct Iterable::instance<List> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return x;
            });
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return list(rest...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            return xs.storage([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }

        template <typename Index, typename Xs>
        static constexpr auto at_impl(Index n, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return detail::at_index::best<n()>(xs...);
            });
        }
    };

    //! @details
    //! `nil<List>` is equivalent to `list()`, and `cons(x, list(xs...))` is
    //! equivalent to `list(x, xs...)`.
    template <>
    struct List::instance<List> : List::mcd<List> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto nil_impl() {
            return list();
        }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return list(x, xs...);
            });
        }

        template <typename Xs, typename Ys>
        static constexpr auto concat_impl(Xs xs, Ys ys) {
            return xs.storage([=](auto ...xs) {
                return ys.storage([=](auto ...ys) {
                    return list(xs..., ys...);
                });
            });
        }

        template <typename Xs>
        static constexpr auto init_impl(Xs xs) {
            return unpack(
                on(list, [=](auto index) { return at(index, xs); }),
                range(size_t<0>, length(xs) - size_t<1>)
            );
        }

        template <typename Xs, typename X>
        static constexpr auto snoc_impl(Xs xs, X x) {
            return xs.storage([=](auto ...xs) {
                return list(xs..., x);
            });
        }

        template <typename N, typename Xs>
        static constexpr auto take_impl(N n, Xs xs) {
            auto min = [](auto a, auto b) { return if_(a < b, a, b); };
            return unpack(
                on(list, [=](auto index) { return at(index, xs); }),
                range(size_t<0>, min(n, length(xs)))
            );
        }

        template <typename F, typename ...Xss>
        static constexpr auto zip_with_impl(F f, Xss ...lists) {
            auto go = [=](auto index) {
                return always(f)(index)(at(index, lists)...);
            };
            auto zip_length = minimum(list(length(lists)...));
            return unpack(on(list, go), range(size_t<0>, zip_length));
        }
    };

    //! @cond
    template <>
    struct Functor::instance<List> : Functor::fmap_mcd {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return list(f(xs)...);
            });
        }
    };
    //! @endcond

    //! @details
    //! Two `List`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //!
    //! ### Example
    //! @snippet example/list/comparable.cpp main
    //!
    //! @todo
    //! Don't use the Iterable instance; write an efficient one.
    template <>
    struct Comparable::instance<List, List> : Iterable::ComparableInstance {

    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_HPP
