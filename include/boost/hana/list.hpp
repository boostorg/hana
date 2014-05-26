/*!
 * @file
 * Defines `boost::hana::List`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_HPP
#define BOOST_HANA_LIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/at_index/best.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/detail/left_folds/variadic.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! @todo How to implement iterate and repeat?
    template <typename Into>
    struct List { Into into; };

    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        auto into = [=](auto f) { return f(xs...); };
        return List<decltype(into)>{into};
    };

    template <typename ...Xs>
    BOOST_HANA_CONSTEXPR_LAMBDA auto list_t = list(Type<Xs>{}...);

    template <typename T, T ...xs>
    BOOST_HANA_CONSTEXPR_LAMBDA auto list_c = list(Integral<T, xs>{}...);

    template <typename Storage>
    struct Iterable<List<Storage>> : defaults<Iterable> {
        static constexpr auto head_impl(List<Storage> xs) {
            return xs.into([](auto x, ...) {
                return x;
            });
        }

        static constexpr auto tail_impl(List<Storage> xs) {
            return xs.into([](auto, auto ...xs) {
                return list(xs...);
            });
        }

        static constexpr auto is_empty_impl(List<Storage> xs) {
            return xs.into([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }

        static constexpr auto length_impl(List<Storage> xs) {
            return xs.into([](auto ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }

        template <typename Index>
        static constexpr auto at_impl(Index n, List<Storage> xs) {
            return xs.into([=](auto ...xs) {
                return detail::at_index::best(n, xs...);
            });
        }
    };

    template <typename Storage>
    struct Functor<List<Storage>> : defaults<Functor> {
        template <typename F>
        static constexpr auto fmap_impl(F f, List<Storage> xs) {
            return xs.into([=](auto ...xs) { return list(f(xs)...); });
        }
    };

    template <typename Storage>
    struct Foldable<List<Storage>> : detail::foldable_from_iterable {
        template <typename F, typename State>
        static constexpr auto foldl_impl(F f, State s, List<Storage> xs) {
            return xs.into([=](auto ...xs) {
                return detail::left_folds::variadic(f, s, xs...);
            });
        }
    };

    template <typename Storage1, typename Storage2>
    struct Comparable<List<Storage1>, List<Storage2>>
        : detail::comparable_from_iterable
    { };

    template <typename Storage1, typename Storage2>
    constexpr auto operator==(List<Storage1> xs, List<Storage2> ys)
    {  return equal(xs, ys); }

    template <typename Storage1, typename Storage2>
    constexpr auto operator!=(List<Storage1> xs, List<Storage2> ys)
    {  return not_equal(xs, ys); }
}} // end namespace boost::hana

#include <boost/hana/range.hpp>

namespace boost { namespace hana {
    namespace list_detail {
        //! @todo Remove this hack.
        template <typename T, T t>
        static constexpr SizeT<t> to_size_t(Integral<T, t>)
        { return {}; }
    }


    BOOST_HANA_CONSTEXPR_LAMBDA auto zip_with = [](auto f, auto ...lists) {
        return fmap(
            [=](auto index) { return f(at(index, lists)...); },
            range(size_t<0>, minimum(list(length(lists)...)))
        );
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto init = [](auto xs) {
        return fmap(
            [=](auto index) { return at(index, xs); },
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


    BOOST_HANA_CONSTEXPR_LAMBDA auto take = [](auto n, auto xs) {
        auto min = [](auto a, auto b) { return if_(a < b, a, b); };
        return fmap(
            [=](auto index) { return at(index, xs); },
            range(size_t<0>, list_detail::to_size_t(min(n, length(xs))))
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
        auto concat2 = [](auto xs, auto ys) {
            return xs.into([=](auto ...xs) {
                return ys.into([=](auto ...ys) {
                    return list(xs..., ys...);
                });
            });
        };
        return foldl(concat2, list(), list(lists...));
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
