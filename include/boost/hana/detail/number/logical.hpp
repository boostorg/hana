/*!
@file
Defines the `boost::hana::Logical` instance for `boost::hana::detail::Number`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_NUMBER_LOGICAL_HPP
#define BOOST_HANA_DETAIL_NUMBER_LOGICAL_HPP

#include <boost/hana/detail/number/number.hpp>

#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Logical::instance<detail::Number<Mcd>>
        : Logical::mcd
    {
        template <typename C, typename T, typename E>
        static constexpr auto eval_if_impl(C c, T t, E e) {
            auto id = [](auto x) { return x; };
            return c.value ? t(id) : e(id);
        }

        template <typename C>
        static constexpr auto not_impl(C c) {
            return c.wrap(!c.value);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_NUMBER_LOGICAL_HPP
