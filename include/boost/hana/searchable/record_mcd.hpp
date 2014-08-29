/*!
@file
Defines `boost::hana::Searchable::record_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_RECORD_MCD_HPP
#define BOOST_HANA_SEARCHABLE_RECORD_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/record/record.hpp>
#include <boost/hana/searchable/mcd.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `Record`
    template <typename R>
    struct Searchable::record_mcd : Searchable::mcd {
        template <typename X, typename Pred>
        static constexpr auto find_impl(X x, Pred pred) {
            return fmap(
                find(members<R>, [=](auto k_f) { return pred(first(k_f)); }),
                [=](auto k_f) { return second(k_f)(x); }
            );
        }

        template <typename X, typename Pred>
        static constexpr auto any_impl(X x, Pred pred) {
            return any(members<R>, [=](auto k_f) { return pred(first(k_f)); });
        }
    };

    //! Searching a `Record` `r` is equivalent to searching `to<Map>(r)`.
    template <typename R>
    struct Searchable::instance<R, when<is_a<Record, R>()>>
        : Searchable::record_mcd<R>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_RECORD_MCD_HPP
