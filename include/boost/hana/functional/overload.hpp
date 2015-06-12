/*!
@file
Defines `boost::hana::overload`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_OVERLOAD_HPP
#define BOOST_HANA_FUNCTIONAL_OVERLOAD_HPP

#include <type_traits>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Pick one of several functions to call based on overload resolution.
    //!
    //! Specifically, `overload(f1, f2, ..., fn)` is a function object such
    //! that
    //! @code
    //!     overload(f1, f2, ..., fn)(x...) == fk(x...)
    //! @endcode
    //!
    //! where `fk` is the function of `f1, ..., fn` that would be called if
    //! overload resolution was performed amongst that set of functions only.
    //! If more than one function `fk` would be picked by overload resolution,
    //! then the call is ambiguous.
    //!
    //! ### Example
    //! @snippet example/functional.cpp overload
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto overload = [](auto&& f1, auto&& f2, ..., auto&& fn) {
        return [perfect-capture](auto&& ...x) -> decltype(auto) {
            return forwarded(fk)(forwarded(x)...);
        };
    };
#else
    template <typename F, typename ...G>
    struct _overload
        : _overload<F>::type
        , _overload<G...>::type
    {
        using type = _overload;
        using _overload<F>::type::operator();
        using _overload<G...>::type::operator();

        template <typename F_, typename ...G_>
        constexpr explicit _overload(F_&& f, G_&& ...g)
            : _overload<F>::type(static_cast<F_&&>(f))
            , _overload<G...>::type(static_cast<G_&&>(g)...)
        { }
    };

    template <typename F>
    struct _overload<F> { using type = F; };

    template <typename R, typename ...Args>
    struct _overload<R(*)(Args...)> {
        using type = _overload;
        R (*fptr_)(Args...);

        explicit constexpr _overload(R (*fp)(Args...))
            : fptr_(fp)
        { }

        constexpr R operator()(Args ...args) const
        { return fptr_(static_cast<Args&&>(args)...); }
    };

    struct _make_overload {
        template <typename ...F,
            typename Overload = typename _overload<
                typename std::decay<F>::type...
            >::type
        >
        constexpr Overload operator()(F&& ...f) const {
            return Overload(static_cast<F&&>(f)...);
        }
    };

    constexpr _make_overload overload{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_OVERLOAD_HPP
