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

namespace boost { namespace hana {
    template <template <typename ...> class f>
    struct _lift {
        template <typename ...Args>
        f<Args...> operator()(Args ...) const;
    };

    template <template <typename ...> class f>
    constexpr auto lift = _lift<f>{};

    constexpr struct _apply {
        template <typename F, typename ...Args>
        constexpr auto operator()(F f, Args ...args) const
        { return f(args...); }
    } apply{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP
