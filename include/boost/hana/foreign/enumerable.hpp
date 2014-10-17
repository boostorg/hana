/*!
@file
Defines the instance of `boost::hana::Enumerable` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_ENUMERABLE_HPP
#define BOOST_HANA_FOREIGN_ENUMERABLE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/enumerable/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Enumerable` for objects of foreign numeric types.
    //!
    //! Any foreign object that can be incremented using `operator++` and
    //! decremented using `operator--` is `Enumerable` using those operations
    //! for `succ` and `pred` respectively.
    template <typename T>
    struct Enumerable::instance<T, when<is_valid<
        decltype((void)(++*(T*)0)),
        decltype((void)(--*(T*)0))
    >>>
        : Enumerable::mcd
    {
        template <typename X>
        static constexpr decltype(auto) succ_impl(X /* by value */ x) {
            return ++x;
        }

        template <typename X>
        static constexpr decltype(auto) pred_impl(X /* by value */ x) {
            return --x;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_ENUMERABLE_HPP
