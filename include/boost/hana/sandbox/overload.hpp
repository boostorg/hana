/*!
@file
Defines `boost::hana::overload`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_OVERLOAD_HPP
#define BOOST_HANA_SANDBOX_OVERLOAD_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana { namespace sandbox {
    namespace overload_detail {
        template <typename ...F>
        struct overload_impl : F... {
            template <typename ...G>
            constexpr explicit overload_impl(G&& ...g)
                : F(detail::std::forward<G>(g))...
            { }
        };

        template <typename F>
        constexpr decltype(auto) wrap_lambda(F&& f) {
            return id(detail::std::forward<F>(f));
        }

        template <typename R, typename ...Args>
        constexpr auto wrap_lambda(R f(Args...)) {
            return [f](Args ...x) -> decltype(f(x...)) {
                return f(detail::std::forward<Args>(x)...);
            };
        }
    } // end namespace overload_detail

    auto overload = [](auto ...f) {
        return overload_detail::overload_impl<
            decltype(overload_detail::wrap_lambda(detail::std::move(f)))...
        >(overload_detail::wrap_lambda(detail::std::move(f))...);
    };
}}} // end namespace boost::hana::sandbox

#endif // !BOOST_HANA_SANDBOX_OVERLOAD_HPP
