/*!
@file
Defines `boost::hana::partial`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
#define BOOST_HANA_FUNCTIONAL_PARTIAL_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Partially apply a function to some arguments.
    //!
    //! Given a function `f` and some arguments, `partial` returns a new
    //! function corresponding to the partially applied function `f`. This
    //! allows providing some arguments to a function and letting the rest
    //! of the arguments be provided later. Specifically, `partial(f, x...)`
    //! is a function such that
    //! @code
    //!     partial(f, x...)(y...) == f(x..., y...)
    //! @endcode
    //!
    //! @note
    //! The arity of `f` must match the total number of arguments passed to
    //! it, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional.cpp partial
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
            return f(static_cast<X const&>(x).get..., static_cast<Y&&>(y)...);
        }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) & {
            return f(static_cast<X&>(x).get..., static_cast<Y&&>(y)...);
        }
#endif

        template <typename ...Y>
        constexpr decltype(auto) operator()(Y&& ...y) && {
            return detail::std::move(f)(
                static_cast<X&&>(x).get..., static_cast<Y&&>(y)...
            );
        }
    };

    struct _make_partial {
        template <typename F, typename ...X>
        constexpr decltype(auto) operator()(F&& f, X&& ...x) const {
            return detail::create<_partial>{}(
                static_cast<F&&>(f),
                detail::create<detail::closure>{}(static_cast<X&&>(x)...)
            );
        }
    };

    constexpr _make_partial partial{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
