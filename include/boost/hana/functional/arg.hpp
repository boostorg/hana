/*!
@file
Defines `boost::hana::arg`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ARG_HPP
#define BOOST_HANA_FUNCTIONAL_ARG_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/variadic/at.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Return the `n`th passed argument.
    //!
    //! Specifically, `arg<n>(x1, ..., xn, ..., xm)` is equivalent to `xn`.
    //! Note that indexing starts at 1, so `arg<1>` returns the 1st argument,
    //! `arg<2>` the 2nd and so on. Using `arg<0>` is an error. Passing
    //! less than `n` arguments to `arg<n>` is also an error.
    //!
    //!
    //! @tparam n
    //! An unsigned integer representing the argument to return. `n` must be
    //! positive (meaning nonzero).
    //!
    //! @param x1, ..., xm
    //! A variadic pack of arguments from which the `n`th one is returned.
    //!
    //!
    //! @internal
    //! ### Discussion: could `n` be dynamic?
    //! We could have chosen `arg` to be used like `arg(n)(x...)` instead of
    //! `arg<n>(x...)`. Provided all the arguments were of the same type, it
    //! would then be possible for `n` to only be known at runtime. However,
    //! we would then lose the ability to assert the in-boundedness of `n`
    //! statically.
    //!
    //! ### Rationale for `n` being a non-type template parameter
    //! I claim that the only interesting use case is with a compile-time
    //! `n`, which means that the usage would become `arg(int_<n>)(x...)`,
    //! which is more cumbersome to write than `arg<n>(x...)`. This is open
    //! for discussion.
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/arg.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto arg = [](auto&& x1, ..., auto&& xm) -> decltype(auto) {
        return forwarded(xn);
    };
#else
    template <detail::std::size_t n>
    struct _arg {
        static_assert(n > 0,
        "invalid usage of boost::hana::arg<n> with n == 0");

        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const {
            static_assert(sizeof...(x) >= n,
            "invalid usage of boost::hana::arg<n> with too few arguments");

            // Since compilers will typically try to continue for a bit after
            // an error/static assertion, we must avoid sending the compiler
            // in a very long computation if n == 0.
            return detail::variadic::at<n == 0 ? 0 : n - 1>(
                detail::std::forward<X>(x)...
            );
        }
    };

    template <detail::std::size_t n>
    constexpr _arg<n> arg{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ARG_HPP
