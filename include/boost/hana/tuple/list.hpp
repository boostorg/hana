/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_LIST_HPP
#define BOOST_HANA_TUPLE_LIST_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/orderable/orderable.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple/tuple.hpp>

// Mcd
#include <boost/hana/tuple/monad.hpp>
#include <boost/hana/tuple/foldable.hpp>
#include <boost/hana/tuple/iterable.hpp>


namespace boost { namespace hana {
    //! Instance of `List` for the `Tuple` data type.
    template <>
    struct List::instance<Tuple> : List::mcd<Tuple> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto nil_impl() {
            return tuple();
        }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return tuple(x, xs...);
            });
        }

        template <typename ...Xs>
        static constexpr auto make_impl(Xs ...xs) {
            return tuple(xs...);
        }

        template <typename Xs, typename Ys>
        static constexpr auto concat_impl(Xs xs, Ys ys) {
            return xs.storage([=](auto ...xs) {
                return ys.storage([=](auto ...ys) {
                    return tuple(xs..., ys...);
                });
            });
        }

        template <typename Xs>
        static constexpr auto init_impl(Xs xs) {
            return unpack(range(size_t<0>, minus(length(xs), size_t<1>)),
                on(tuple, [=](auto index) { return at(index, xs); })
            );
        }

        template <typename Xs, typename X>
        static constexpr auto snoc_impl(Xs xs, X x) {
            return xs.storage([=](auto ...xs) {
                return tuple(xs..., x);
            });
        }

        template <typename N, typename Xs>
        static constexpr auto take_impl(N n, Xs xs) {
            return unpack(range(size_t<0>, min(n, length(xs))),
                on(tuple, [=](auto index) { return at(index, xs); })
            );
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

#endif // !BOOST_HANA_TUPLE_LIST_HPP
