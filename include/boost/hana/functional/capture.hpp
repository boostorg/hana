/*!
@file
Defines `boost::hana::capture`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_CAPTURE_HPP
#define BOOST_HANA_FUNCTIONAL_CAPTURE_HPP

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/functional/partial.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


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
    //! Example
    //! -------
    //! @include example/functional/capture.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto capture = [](auto&& ...variables) {
        return [perfect-capture](auto&& f) {
            return [perfect-capture](auto&& ...args) -> decltype(auto) {
                return forwarded(f)(forwarded(variables)..., forwarded(args)...);
            };
        };
    };
#else
    namespace detail {
        template <typename F, typename Closure, std::size_t ...i>
        constexpr auto apply_capture(F&& f, Closure&& closure, std::index_sequence<i...>) {
            return hana::partial(static_cast<F&&>(f),
                hana::get_impl<i>(static_cast<Closure&&>(closure).storage_)...
            );
        }
    }

    template <typename ...X>
    struct capture_t;

    struct make_capture_t {
        struct secret { };
        template <typename ...X>
        constexpr capture_t<typename std::decay<X>::type...>
        operator()(X&& ...x) const {
            return {secret{}, static_cast<X&&>(x)...};
        }
    };

    template <typename ...X>
    struct capture_t {
        template <typename ...Y>
        constexpr capture_t(make_capture_t::secret, Y&& ...y)
            : storage_{static_cast<Y&&>(y)...}
        { }

        basic_tuple<X...> storage_;

        template <typename F>
        constexpr auto operator()(F&& f) const& {
            return detail::apply_capture(
                static_cast<F&&>(f), *this,
                std::make_index_sequence<sizeof...(X)>{}
            );
        }

        template <typename F>
        constexpr auto operator()(F&& f) & {
            return detail::apply_capture(
                static_cast<F&&>(f), *this,
                std::make_index_sequence<sizeof...(X)>{}
            );
        }

        template <typename F>
        constexpr auto operator()(F&& f) && {
            return detail::apply_capture(
                static_cast<F&&>(f), static_cast<capture_t&&>(*this),
                std::make_index_sequence<sizeof...(X)>{}
            );
        }
    };

    constexpr make_capture_t capture{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_CAPTURE_HPP
