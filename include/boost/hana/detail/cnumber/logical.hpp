/*!
@file
Defines the `boost::hana::Logical` instance for `boost::hana::detail::CNumber`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CNUMBER_LOGICAL_HPP
#define BOOST_HANA_DETAIL_CNUMBER_LOGICAL_HPP

#include <boost/hana/detail/cnumber/cnumber.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    namespace cnumber_detail {
        template <bool b>
        BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if_helper = [](auto t, auto e) {
            return t([](auto x) { return x; });
        };

        template <>
        BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if_helper<false> = [](auto t, auto e) {
            return e([](auto x) { return x; });
        };
    }

    template <typename Mcd>
    struct Logical::instance<detail::CNumber<Mcd>>
        : Logical::mcd
    {
        template <typename X>
        static constexpr auto not_impl(X const&) {
            return X::template wrap<bool, !X::value>;
        }

        template <typename C, typename T, typename E>
        static constexpr auto eval_if_impl(C c, T t, E e) {
            return cnumber_detail::eval_if_helper<C::value>(t, e);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_CNUMBER_LOGICAL_HPP
