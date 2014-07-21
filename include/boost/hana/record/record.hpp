/*!
@file
Forward declares `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_RECORD_HPP
#define BOOST_HANA_RECORD_RECORD_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Type class for record-like user-defined types.
    //!
    //! A record-like type is any type which is fundamentally a `struct`,
    //! regardless of the implementation of its accessors and other similar
    //! details.
    //!
    //! ### Example
    //! @include example/record/howto.cpp
    struct Record {
        BOOST_HANA_TYPECLASS(Record);
        struct mcd;
    };

    //! A compile-time `Map` representing the data structure.
    //! @method{Record}
    //!
    //! Specifically, `members<R>` is a `Map` associating keys to functions,
    //! where a pair `(k, f)` means that the member represented by the key `k`
    //! can be accessed by calling the function `f` on an object of data type
    //! `R`.
    template <typename R>
    BOOST_HANA_CONSTEXPR_LAMBDA auto members = Record::instance<R>::members_impl();
}} // end namespace boost::hana


#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp> // for instantiates
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/searchable/find_mcd.hpp>


namespace boost { namespace hana {
    template <typename R>
    struct Comparable::instance<R, R, when<instantiates<Record, R>()>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return all([=](auto accessor) {
                return equal(accessor(x), accessor(y));
            }, values(members<R>));
        }
    };

    template <typename R>
    struct Foldable::instance<R, when<instantiates<Record, R>()>>
        : Foldable::mcd
    {
        template <typename F, typename S, typename X>
        static constexpr auto foldl_impl(F f, S s, X x) {
            auto g = [=](auto s, auto accessor) {
                return f(s, accessor(x));
            };
            return foldl(g, s, values(members<R>));
        }

        template <typename F, typename S, typename X>
        static constexpr auto foldr_impl(F f, S s, X x) {
            auto g = [=](auto accessor, auto s) {
                return f(accessor(x), s);
            };
            return foldr(g, s, values(members<R>));
        }
    };

    template <typename R>
    struct Searchable::instance<R, when<instantiates<Record, R>()>>
        : Searchable::find_mcd
    {
        template <typename Pred, typename X>
        static constexpr auto find_impl(Pred pred, X x) {
            auto maybe_accessor = find(pred, members<R>);
            return fmap([=](auto accessor) { return accessor(x); }, maybe_accessor);
        }

        template <typename Pred, typename X>
        static constexpr auto any_impl(Pred pred, X x) {
            return any(pred, members<R>);
        }
    };

    template <typename R>
    struct convert<Map, R, detail::enable_if_t<instantiates<Record, R>()>> {
        template <typename X>
        static constexpr auto apply(X x) {
            return to<Map>(
                foldr([=](auto key, auto rest) {
                    return cons(pair(key, from_just(lookup(key, x))), rest);
                }, list(), keys(members<R>))
            );
        }
    };
}} // end namespace boost::hana


#endif // !BOOST_HANA_RECORD_RECORD_HPP
