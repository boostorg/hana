/*!
@file
Defines the instance of `boost::hana::Logical` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_LOGICAL_HPP
#define BOOST_HANA_FOREIGN_LOGICAL_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Logical` for objects of foreign types that can be
    //! implicitly converted to `bool`.
    //!
    //! Any foreign object that can be converted to `bool` implicitly is an
    //! instance of `Logical` by converting that object to `bool` and then
    //! using the obvious instance for `bool`.
    template <typename L>
    struct Logical::instance<L,
        when<is_valid<decltype(*(L*)0 ? (void)0 : (void)0)>>
    >
        : Logical::mcd
    {
        //! @bug
        //! We can't use perfect forwarding because of this bug:
        //! http://llvm.org/bugs/show_bug.cgi?id=20619
        template <typename T, typename E>
        static constexpr auto eval_if_impl(bool cond, T t, E e) {
            return cond ? t(id) : e(id);
        }

        static constexpr auto not_impl(bool cond)
        { return !cond; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_LOGICAL_HPP
