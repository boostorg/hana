/*!
@file
Defines `boost::hana::detail::reverse_partial`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_REVERSE_PARTIAL_HPP
#define BOOST_HANA_DETAIL_REVERSE_PARTIAL_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Reverse version of `partial` from the @ref Functional utilities.
    //!
    //! Specifically, `reverse_partial(f, x...)(y...)` is equivalent to
    //! `f(y..., x...)`.
    //!
    //! @todo
    //! Perhaps find a better name and make this part of Functional.
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
    struct _reverse_partial<F, detail::closure<X...>> {
        F f;
        detail::closure<X...> x;

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) const& {
            return f(detail::std::forward<Y>(y)..., static_cast<X const&>(x).get...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) & {
            return f(detail::std::forward<Y>(y)..., static_cast<X&>(x).get...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) && {
            return detail::std::move(f)(
                detail::std::forward<Y>(y)..., static_cast<X&&>(x).get...
            );
        }
    };

    struct _make_reverse_partial {
        template <typename F, typename ...X>
        constexpr decltype(auto) operator()(F&& f, X&& ...x) const {
            return detail::create<_reverse_partial>{}(
                detail::std::forward<F>(f),
                detail::create<detail::closure_t>{}(detail::std::forward<X>(x)...)
            );
        }
    };

    constexpr _make_reverse_partial reverse_partial{};
#endif
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_REVERSE_PARTIAL_HPP
