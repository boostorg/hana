/*!
@file
Defines the instance of `boost::hana::Logical` for `boost::hana::Foreign`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_LOGICAL_HPP
#define BOOST_HANA_FOREIGN_LOGICAL_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Logical` for `Foreign` objects that can be implicitly
    //! converted to `bool`.
    //!
    //! Any `Foreign` object that can be converted to `bool` implicitly is an
    //! instance of `Logical` by converting that object to `bool` and then
    //! using the obvious instance for `bool`.
    template <typename T>
    struct Logical::instance<Foreign<T>,
        when<is_valid<decltype(*(T*)0 ? (void)0 : (void)0)>>
    >
        : Logical::mcd
    {
        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(bool cond, Then t, Else e) {
            auto id = [](auto x) { return x; };
            return cond ? t(id) : e(id);
        }

        static constexpr auto not_impl(bool cond)
        { return !cond; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_LOGICAL_HPP
