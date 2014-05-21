/*!
 * @file
 * Defines functional utilities.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_HPP
#define BOOST_HANA_FUNCTIONAL_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    template <template <typename ...> class f>
    struct _lift {
        template <typename ...Args>
        f<Args...> operator()(Args ...) const;
    };

    template <template <typename ...> class f>
    constexpr auto lift = _lift<f>{};


    BOOST_HANA_CONSTEXPR_LAMBDA auto apply = [](auto f, auto ...xs) {
        return f(xs...);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto always = [](auto x) {
        return [=](auto ...y) { return x; };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto x) { return x; };

    BOOST_HANA_CONSTEXPR_LAMBDA auto compose = [](auto f, auto g) {
        return [=](auto x, auto ...xs) { return f(g(x), xs...); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto flip = [](auto f) {
        return [=](auto x, auto y, auto ...z) { return f(y, x, z...); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto partial = [](auto f, auto ...xs) {
        return [=](auto ...ys) { return f(xs..., ys...); };
    };

    constexpr struct {
        template <typename F>
        constexpr auto operator()(F f) const {
            auto fst = [](auto a, ...) { return a; };
            //! @todo Find a simpler way to make `f` dependent.
            return [=](auto ...xs) {
                return f((*this)(fst(f, xs...)), xs...);
            };
        }
    } fix{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP
