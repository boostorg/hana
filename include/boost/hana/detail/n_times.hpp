/*!
@file
Defines `boost::hana::detail::n_times`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_N_TIMES_HPP
#define BOOST_HANA_DETAIL_N_TIMES_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail {
    template <detail::std::size_t n>
    struct n_times_impl {
        template <typename F, detail::std::size_t ...i>
        static constexpr void
        go(F&& f, detail::std::integer_sequence<detail::std::size_t, i...>)
        {
            using swallow = detail::std::size_t[];
            (void)swallow{1,
                (f(), void(), i)...
            };
        }

        template <typename F>
        constexpr void operator()(F&& f) const {
            go(
                detail::std::forward<F>(f),
                detail::std::make_integer_sequence<detail::std::size_t, n>{}
            );
        }
    };

    template <detail::std::size_t n>
    constexpr n_times_impl<n> n_times{};
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_N_TIMES_HPP
