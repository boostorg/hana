/*!
@file
Forward declares `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_LOGICAL_HPP

#include <boost/hana/logical/detail/logical_fwd.hpp>

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `eval_if` and `not_`
    struct Logical::mcd {
        template <typename X, typename Y>
        static constexpr auto or_impl(X x, Y y)
        { return if_(x, x, y); }

        template <typename X, typename Y>
        static constexpr auto and_impl(X x, Y y)
        { return if_(x, y, x); }

        template <typename C, typename T, typename E>
        static constexpr auto if_impl(C c, T t, E e) {
            return eval_if(c,
                [=](auto) { return t; },
                [=](auto) { return e; }
            );
        }
    };

    //! Instance of `Logical` for objects with a data type that can be
    //! converted to `bool` implicitly.
    //!
    //! Any object whose data type can be converted to `bool` implicitly is
    //! an instance of `Logical` by converting that object to `bool` and then
    //! using the obvious instance for `bool`.
    template <typename T>
    struct Logical::instance<T, when_valid<decltype(*(T*)0 ? (void)0 : (void)0)>>
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

#endif // !BOOST_HANA_LOGICAL_LOGICAL_HPP
