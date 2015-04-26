/*!
@file
Defines `boost::hana::capture`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_CAPTURE_HPP
#define BOOST_HANA_FUNCTIONAL_CAPTURE_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Create a function capturing the given variables.
    //!
    //! Given 0 or more variables and a function `f`, `capture` returns a
    //! function equivalent to `f` except that the first arguments sent to
    //! `f` are the captured variables. Specifically, `capture(vars...)(f)`
    //! is a function such that
    //! @code
    //!     capture(vars...)(f)(args...) == f(vars..., args...)
    //! @endcode
    //!
    //!
    //! ### Example
    //! @snippet example/functional.cpp capture
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto capture = [](auto&& ...variables) {
        return [perfect-capture](auto&& f) {
            return [perfect-capture](auto&& ...args) -> decltype(auto) {
                return forwarded(f)(forwarded(variables)..., forwarded(args)...);
            };
        };
    };
#else
    template <typename X>
    struct _capture;

    template <typename ...X>
    struct _capture<detail::closure_impl<X...>> {
        detail::closure_impl<X...> x;

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const& {
            return hana::partial(static_cast<F&&>(f),
                                 static_cast<X const&>(x).get...);
        }

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) & {
            return hana::partial(static_cast<F&&>(f),
                                 static_cast<X&>(x).get...);
        }

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) && {
            return hana::partial(static_cast<F&&>(f),
                                 static_cast<X&&>(x).get...);
        }
    };

    struct _make_capture {
        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const {
            return detail::create<_capture>{}(
                detail::create<detail::closure>{}(static_cast<X&&>(x)...)
            );
        }
    };

    constexpr _make_capture capture{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_CAPTURE_HPP
