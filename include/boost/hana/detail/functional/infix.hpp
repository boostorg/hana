/*!
@file
Defines `boost::hana::infix`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_FUNCTIONAL_INFIX_HPP
#define BOOST_HANA_DETAIL_FUNCTIONAL_INFIX_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    namespace infix_detail {
        template <typename F, bool left = false, bool right = false>
        struct infix {
            F f;

            template <typename ...X>
            constexpr auto operator()(X ...x) const
            { return f(x...); }
        };

        // infix(f) ^ y
        template <typename F, bool left, bool right, typename Y>
        constexpr auto operator^(infix<F, left, right> f, Y y) {
            static_assert(!right,
            "invalid usage of boost::hana::infix with a double right operand");

            auto g = [=](auto ...x) { return f(x..., y); };
            return infix<decltype(g), left, true>{g};
        }

        template <typename F, bool right, typename Y>
        constexpr auto operator^(infix<F, true, right> f, Y y) {
            static_assert(!right,
            "invalid usage of boost::hana::infix with a double right operand");

            return f(y);
        }

        // x ^ infix(f)
        template <typename X, typename F, bool left, bool right>
        constexpr auto operator^(X x, infix<F, left, right> f) {
            static_assert(!left,
            "invalid usage of boost::hana::infix with a double left operand");

            auto g = [=](auto ...y) { return f(x, y...); };
            return infix<decltype(g), true, right>{g};
        }

        template <typename X, typename F, bool left>
        constexpr auto operator^(X x, infix<F, left, true> f) {
            static_assert(!left,
            "invalid usage of boost::hana::infix with a double left operand");

            return f(x);
        }
    } // end namespace infix_detail

#ifndef BOOST_HANA_DOXYGEN_INVOKED
    BOOST_HANA_CONSTEXPR_LAMBDA auto infix = [](auto f) {
        return infix_detail::infix<decltype(f)>{f};
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_FUNCTIONAL_INFIX_HPP
