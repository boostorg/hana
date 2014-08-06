/*!
@file
Defines `boost::hana::curry`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_FUNCTIONAL_CURRY_HPP
#define BOOST_HANA_DETAIL_FUNCTIONAL_CURRY_HPP

#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana {
    namespace curry_detail {
        template <typename F, detail::std::size_t arity>
        struct curry {
            F f;

            template <typename ...X>
            constexpr auto operator()(X ...x) const {
                static_assert(arity >= sizeof...(x),
                "too many arguments provided to boost::hana::curry");

                auto g = [f = f, x...](auto ...y) { return f(x..., y...); };
                return curry<decltype(g), arity - sizeof...(x)>{g}();
            }

            constexpr auto operator()() const
            { return *this; }
        };

        template <typename F>
        struct curry<F, 0> {
            F f;

            constexpr auto operator()() const
            { return f(); }
        };

        template <detail::std::size_t arity>
        struct make_curry {
            template <typename F>
            constexpr auto operator()(F f) const
            { return curry<F, arity>{f}; }
        };
    } // end namespace curry_detail

#ifndef BOOST_HANA_DOXYGEN_INVOKED
    template <detail::std::size_t arity>
    constexpr curry_detail::make_curry<arity> curry{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_FUNCTIONAL_CURRY_HPP
