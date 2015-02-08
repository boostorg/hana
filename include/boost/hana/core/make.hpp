/*!
@file
Defines `boost::hana::make`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_MAKE_HPP
#define BOOST_HANA_CORE_MAKE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Create an object of the given data type with the given arguments.
    //!
    //! This function serves the same purpose as constructors in usual C++.
    //! However, instead of creating an object of a specific C++ type, it
    //! creates an object of a specific data type, with a C++ type that is
    //! left unspecified in the general case.
    //!
    //! This variable template may be customized by specializing it for a
    //! specific data type. When left unspecialized, `make<Datatype>(args...)`
    //! defaults to `Datatype(args...)` if that expression is valid, which is
    //! useful because it makes foreign C++ types constructible with `make`
    //! out-of-the-box. When the `Datatype(args...)` is invalid and no
    //! specialization is provided, a static assertion is triggered.
    //!
    //! ### Example
    //! @snippet example/core/make.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Datatype>
    constexpr auto make = [](auto&& ...x) -> decltype(auto) {
        return make_impl<Datatype>::apply(
            std::forward<decltype(x)>(x)...
        );
    };
#else
    template <typename Datatype, typename = void>
    struct make_impl : make_impl<Datatype, when<true>> { };

    template <typename Datatype, bool condition>
    struct make_impl<Datatype, when<condition>> {
        template <typename ...X>
        static constexpr auto make_helper(int, X&& ...x)
            -> decltype(Datatype(detail::std::forward<X>(x)...))
        { return Datatype(detail::std::forward<X>(x)...); }

        template <typename ...X>
        static constexpr auto make_helper(long, X&& ...) {
            static_assert((sizeof...(X), false),
            "there exists no constructor for the given data type");
        }

        template <typename ...X>
        static constexpr decltype(auto) apply(X&& ...x)
        { return make_helper(int{}, detail::std::forward<X>(x)...); }
    };

    template <typename Datatype>
    struct _make {
        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const {
            return make_impl<Datatype>::apply(detail::std::forward<X>(x)...);
        }
    };

    template <typename Datatype>
    constexpr _make<Datatype> make{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MAKE_HPP
