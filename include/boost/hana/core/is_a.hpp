/*!
@file
Defines `boost::hana::is_a`, `boost::hana::is_an`, and `boost::hana::are`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_IS_A_HPP
#define BOOST_HANA_CORE_IS_A_HPP

#include <boost/hana/bool/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/disable.hpp>


namespace boost { namespace hana {
    //! @anchor group-core-is_a
    //! @ingroup group-core
    //! Return whether a type class is instantiated with the given data types.
    //!
    //! Specifically, `is_a<Typeclass, Datatypes...>` is a [compile-time]
    //! (@ref Logical_terminology) `Logical` representing whether
    //! `Typeclass::instance<Datatypes...>` represents an instance of
    //! `Typeclass`. This does not check whether the instance is valid: it
    //! does not check whether laws are satisfied or even if the minimal
    //! complete definition is syntactically correct. It only tells whether
    //! an instance is there and was not disabled explicitly with `disable`.
    //! Note that exactly one or two data types must be provided because the
    //! library currently supports only unary and binary type classes; failure
    //! to provide a correct number of arguments will trigger a static
    //! assertion.
    //!
    //! ### Example
    //! @snippet example/core/is_a.cpp standard
    //!
    //! An alternate syntax is provided for convenience. `is_a` can also be
    //! used as a function returning whether the data types of its arguments
    //! together define an instance of the given type class. In other words,
    //! @code
    //!     is_a<Typeclass>(x1, ..., xn) == is_a<Typeclass, D1, ..., Dn>
    //! @endcode
    //! where `xi` is an arbitrary object and `Di` is the data type of the
    //! corresponding `xi`.
    //!
    //! With this alternate syntax, if a data type `D` is provided instead of
    //! a type class, and if a single object is passed, it returns whether the
    //! passed object has `D` as a data type. In other words,
    //! @code
    //!     is_a<Datatype>(x)
    //! @endcode
    //! is true if `x` has a data type of `Datatype`.
    //!
    //! @note
    //! This relies on the fact that data types do not coincide with type
    //! classes. Otherwise, it would be impossible to differentiate between
    //! `is_a<Datatype>` and `is_a<Typeclass>` without introducing additional
    //! machinery.
    //!
    //! ### Example
    //! @snippet example/core/is_a.cpp alternate
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto is_a = unspecified;
#else
    namespace core_detail {
        constexpr auto is_a_impl(...)      { return true_; }
        constexpr auto is_a_impl(disable*) { return false_; }
    }

    template <typename Typeclass, typename ...Datatypes>
    constexpr auto is_a = core_detail::is_a_impl(
        (typename Typeclass::template instance<Datatypes...>*)0
    );

    namespace core_detail {
        template <typename Datatype, typename ...D>
        constexpr auto is_a_for_datatype = is_a<Datatype, D...>;

        template <typename Datatype>
        constexpr auto is_a_for_datatype<Datatype, Datatype> = true_;

        template <typename Typeclass>
        struct is_a_helper {
            template <typename ...X>
            constexpr auto operator()(X const& ...) const {
                static_assert(sizeof...(X) >= 1,
                "invalid usage of is_a<Typeclass> with 0 arguments; "
                "it requires either one or two arguments");

                static_assert(sizeof...(X) <= 2,
                "invalid usage of is_a<Typeclass> with more than two "
                "arguments; it requires either one or two arguments");

                return is_a_for_datatype<Typeclass, typename datatype<X>::type...>;
            }
        };
    }

    template <typename Typeclass>
    constexpr core_detail::is_a_helper<Typeclass> is_a<Typeclass>{};
#endif

    //! @ingroup group-core
    //! Equivalent to `is_a`; provided for consistency with the rules of the
    //! English language.
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto is_an = is_a<Typeclass, Datatypes...>;

    //! @ingroup group-core
    //! Equivalent to `is_a`; provided for consistency with the rules of the
    //! English language when several data types are provided.
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto are = is_a<Typeclass, Datatypes...>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_IS_A_HPP

#include <boost/hana/bool.hpp>
