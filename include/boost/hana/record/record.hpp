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

    //! A list of pairs representing the data structure.
    //! @method{Record}
    //!
    //! Specifically, `members<R>` is a `List` of `Pair`s associating keys to
    //! functions, where a pair `(k, f)` means that the member represented by
    //! the key `k` can be accessed by calling the function `f` on an object
    //! of data type `R`.
    template <typename R>
    BOOST_HANA_CONSTEXPR_LAMBDA auto members = Record::instance<R>::members_impl();
}} // end namespace boost::hana


#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp> // for instantiates
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair/instance.hpp>
#include <boost/hana/searchable/find_mcd.hpp>


namespace boost { namespace hana {
    //! Two `Records` of the same data type `R` are equal if and only if
    //! all their members are equal. The members are compared in the
    //! same order as they appear in `members<R>`.
    template <typename R>
    struct Comparable::instance<R, R, when<instantiates<Record, R>()>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return all([=](auto k_f) {
                return equal(second(k_f)(x), second(k_f)(y));
            }, members<R>);
        }
    };

    //! Folding a `Record` `R` is equivalent to folding a list of its members,
    //! in the same order as they appear in `members<R>`.
    template <typename R>
    struct Foldable::instance<R, when<instantiates<Record, R>()>>
        : Foldable::mcd
    {
        template <typename F, typename S, typename X>
        static constexpr auto foldl_impl(F f, S s, X x) {
            auto g = [=](auto s, auto k_f) {
                return f(s, second(k_f)(x));
            };
            return foldl(g, s, members<R>);
        }

        template <typename F, typename S, typename X>
        static constexpr auto foldr_impl(F f, S s, X x) {
            auto g = [=](auto k_f, auto s) {
                return f(second(k_f)(x), s);
            };
            return foldr(g, s, members<R>);
        }
    };

    //! Searching a `Record` `r` is equivalent to searching `to<Map>(r)`.
    template <typename R>
    struct Searchable::instance<R, when<instantiates<Record, R>()>>
        : Searchable::find_mcd
    {
        template <typename Pred, typename X>
        static constexpr auto find_impl(Pred pred, X x) {
            return fmap(
                [=](auto k_f) { return second(k_f)(x); },
                find([=](auto k_f) { return pred(first(k_f)); }, members<R>)
            );
        }

        template <typename Pred, typename X>
        static constexpr auto any_impl(Pred pred, X x) {
            return any([=](auto k_f) { return pred(first(k_f)); }, members<R>);
        }
    };

    //! Converting a `Record` `R` to a `Map` is equivalent to converting its
    //! `members<R>` to a `Map`, except the values are replaced by the actual
    //! members of the object instead of accessors.
    template <typename R>
    struct convert<Map, R, detail::enable_if_t<instantiates<Record, R>()>> {
        template <typename X>
        static constexpr auto apply(X x) {
            return to<Map>(fmap(
                [=](auto k_f) { return pair(first(k_f), second(k_f)(x)); },
                members<R>
            ));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_RECORD_HPP
