/*!
@file
Forward declares `boost::hana::make`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CORE_MAKE_HPP
#define BOOST_HANA_FWD_CORE_MAKE_HPP

namespace boost { namespace hana {
    //! @ingroup group-core
    //! Create an object of the given data type with the given arguments.
    //!
    //! This function serves the same purpose as constructors in usual C++.
    //! However, instead of creating an object of a specific C++ type, it
    //! creates an object of a specific data type, with a C++ type that is
    //! left unspecified in the general case.
    //!
    //! This function is actually a variable template, so `make<T>` can be
    //! passed around as a function object creating an object of data type
    //! `T`. Also, it uses tag-dispatching so this is how it should be
    //! customized for user-defined data types.
    //!
    //! Finally, the default implementation of `make` is equivalent
    //! to calling the constructor of the given data type with the
    //! corresponding arguments. In other words, by default,
    //! @code
    //!     make<T>(args...) == T(args...)
    //! @endcode
    //!
    //! Note that the arguments are perfectly forwarded and the form of
    //! construction which is used is exactly as documented, i.e. `T(args...)`.
    //! However, if `T(args...)` is not a valid expression, a compilation
    //! error is triggered. This default behavior is useful because it makes
    //! foreign C++ types that have no notion of _data type_ constructible
    //! with `make` out-of-the-box, since their data type is exactly
    //! themselves.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/make.cpp make
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Datatype>
    constexpr auto make = [](auto&& ...x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Datatype, typename = void>
    struct make_impl;

    template <typename Datatype>
    struct make_t {
        template <typename ...X>
        constexpr decltype(auto) operator()(X&& ...x) const {
            return make_impl<Datatype>::apply(static_cast<X&&>(x)...);
        }
    };

    template <typename Datatype>
    constexpr make_t<Datatype> make{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CORE_MAKE_HPP
