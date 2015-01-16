/*!
@file
Defines `boost::hana::make`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_MAKE_HPP
#define BOOST_HANA_CORE_MAKE_HPP

#include <boost/hana/core/method.hpp>
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
    //! By default, `make<Datatype>(args...)` is equivalent to
    //! `Datatype(args...)` if that expression is valid, which is
    //! useful because it makes foreign C++ types constructible with
    //! `make` out-of-the-box. When `Datatype(args...)` is invalid, a
    //! compile-time error is triggered.
    //!
    //! ### Example
    //! @snippet example/core/make.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Datatype>
    constexpr auto make = [](auto&& ...args) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(make_impl);

    template <typename Datatype>
    struct _make {
        template <typename ...Args>
        constexpr decltype(auto) operator()(Args&& ...args) const {
            return dispatch<make_impl<Datatype>>::apply(
                detail::std::forward<Args>(args)...
            );
        }
    };

    template <typename Datatype>
    constexpr _make<Datatype> make{};

    template <typename Datatype, typename Context>
    struct dispatch_impl<4, make_impl<Datatype>, Context> {
        using type = dispatch_impl;
        template <typename ...X>
        static constexpr auto apply(X&& ...x)
            -> decltype(Datatype(detail::std::forward<X>(x)...))
        { return Datatype(detail::std::forward<X>(x)...); }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MAKE_HPP
