/*!
@file
Defines the instance of `boost::hana::Logical` for `boost::hana::Bool`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_LOGICAL_HPP
#define BOOST_HANA_BOOL_LOGICAL_HPP

#include <boost/hana/bool/bool.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    template <>
    struct Logical::instance<Bool> : Logical::mcd {
        //! @bug
        //! We can't use perfect forwarding because of this bug:
        //! http://llvm.org/bugs/show_bug.cgi?id=20619
        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(true_), T t, E)
        { return t(id); }

        template <typename T, typename E>
        static constexpr auto eval_if_impl(decltype(false_), T, E e)
        { return e(id); }

        template <typename C>
        static constexpr auto not_impl(C const&)
        { return bool_<!C::value>; }
    };
}}

#endif // !BOOST_HANA_BOOL_LOGICAL_HPP
