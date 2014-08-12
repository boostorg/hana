/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_COMPARABLE_HPP
#define BOOST_HANA_TYPE_LIST_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/type_list/type_list.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<TypeList, TypeList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys)
        { return false_; }

        template <typename Xs>
        static constexpr auto equal_impl(Xs, Xs)
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_COMPARABLE_HPP
