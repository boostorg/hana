/*!
@file
Defines `boost::hana::reverse_partial`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP
#define BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Partially apply a function to some arguments.
    //!
    //! Given a function `f` and some arguments, `reverse_partial` returns a
    //! new function corresponding to `f` whose last arguments are partially
    //! applied. Specifically, `reverse_partial(f, x...)` is a function such
    //! that
    //! @code
    //!     reverse_partial(f, x...)(y...) == f(y..., x...)
    //! @endcode
    //!
    //! @note
    //! The arity of `f` must match the total number of arguments passed to
    //! it, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional.cpp reverse_partial
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse_partial = [](auto&& f, auto&& ...x) {
        return [perfect-capture](auto&& ...y) -> decltype(auto) {
            return forwarded(f)(forwarded(y)..., forwarded(x)...);
        };
    };
#else
    template <typename F, typename X>
    struct _reverse_partial;

    template <typename F, typename ...X>
    struct _reverse_partial<F, detail::closure_impl<X...>> {
        F f;
        detail::closure_impl<X...> x;

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) const& {
            return f(static_cast<Y&&>(y)..., static_cast<X const&>(x).get...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) & {
            return f(static_cast<Y&&>(y)..., static_cast<X&>(x).get...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) && {
            return detail::std::move(f)(
                static_cast<Y&&>(y)..., static_cast<X&&>(x).get...
            );
        }
    };

    struct _make_reverse_partial {
        template <typename F, typename ...X>
        constexpr decltype(auto) operator()(F&& f, X&& ...x) const {
            return detail::create<_reverse_partial>{}(
                static_cast<F&&>(f),
                detail::create<detail::closure>{}(static_cast<X&&>(x)...)
            );
        }
    };

    constexpr _make_reverse_partial reverse_partial{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_REVERSE_PARTIAL_HPP
