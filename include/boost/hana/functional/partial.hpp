/*!
@file
Defines `boost::hana::partial`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
#define BOOST_HANA_FUNCTIONAL_PARTIAL_HPP

#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Partially apply a function to some arguments.
    //!
    //! Specifically, `partial(f, x...)` is a function such that
    //! @code
    //!     partial(f, x...)(y...) == f(x..., y...)
    //! @endcode
    //!
    //! Given the semantics, the arity of `f` must match the number of
    //! arguments passed in total, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //! ### Example
    //! @snippet example/functional/partial.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto partial = [](auto&& f, auto&& ...x) {
        return [perfect-capture](auto&& ...y) -> decltype(auto) {
            return forwarded(f)(forwarded(x)..., forwarded(y)...);
        };
    };
#else
    template <typename F, typename X>
    struct _partial;

    template <typename F, typename ...X>
    struct _partial<F, detail::closure_impl<X...>> {
        F f;
        detail::closure_impl<X...> x;

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) const& {
            return f(static_cast<X const&>(x).get..., detail::std::forward<Y>(y)...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) & {
            return f(static_cast<X&>(x).get..., detail::std::forward<Y>(y)...);
        }

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) && {
            return detail::std::move(f)(
                static_cast<X&&>(x).get..., detail::std::forward<Y>(y)...
            );
        }
    };

    struct _make_partial {
        template <typename F, typename ...X>
        constexpr decltype(auto) operator()(F&& f, X&& ...x) const {
            return detail::create<_partial>{}(
                detail::std::forward<F>(f),
                detail::create<detail::closure>{}(detail::std::forward<X>(x)...)
            );
        }
    };

    constexpr _make_partial partial{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
