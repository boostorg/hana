/*!
@file
Defines the @ref core module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_HPP
#define BOOST_HANA_CORE_HPP

#include <boost/hana/detail/integral_fwd.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @defgroup core Core
    //! Miscellaneous core utilities.

    namespace core_detail {
        constexpr auto instantiates_impl(...) { return true_; }
        constexpr auto instantiates_impl(disable*) { return false_; }
    }

    //! @ingroup core
    //! Whether the type class is instantiated with the given arguments.
    //! @hideinitializer
    //!
    //! This is provided in addition to `is_a` for type classes taking more
    //! than one argument or when no object of the data type is available.
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto instantiates = core_detail::instantiates_impl(
        (typename Typeclass::template instance<Datatypes...>*)0
    );

    namespace core_detail {
        template <typename Typeclass>
        struct is_a {
            template <typename X>
            constexpr auto operator()(X x) const
            { return instantiates<Typeclass, datatype_t<X>>; }
        };
    }

    //! @ingroup core
    //! Return whether an object is an instance of the given type class.
    //!
    //! ### Example
    //! @snippet example/core/is_a.cpp main
    template <typename Typeclass>
    constexpr core_detail::is_a<Typeclass> is_a{};

    //! @ingroup core
    //! Equivalent to `is_a`; provided for consistency with the rules of the
    //! English language.
    template <typename Typeclass>
    constexpr core_detail::is_a<Typeclass> is_an{};

    namespace core_detail {
        template <typename To, typename From>
        struct default_convert {
            template <typename X>
            static constexpr auto apply_impl(X x, int)
                -> decltype(static_cast<To>(x))
            { return static_cast<To>(x); }

            template <typename X>
            static constexpr auto apply_impl(X x, ...) {
                static_assert((sizeof(X), false),
                "there exists no conversion between the given data types");
            }

            template <typename X>
            static constexpr auto apply(X x)
            { return apply_impl(x, int{0}); }
        };

        template <typename To>
        struct default_convert<To, To> {
            template <typename X>
            static constexpr auto apply(X x) { return x; }
        };
    }

    //! @ingroup core
    //! Implements conversions between data types.
    //!
    //! To specify a conversion between two data types, one must specialize
    //! `convert` for the corresponding data types. A dummy template parameter
    //! is also provided for SFINAE. This allows conversions to be specified
    //! for all data types satisfying a predicate.
    //!
    //! By default, `convert` has the following behavior:
    //! If the `To` and `From` data types are the same, nothing is done.
    //! Otherwise, if the type of the converted-from object -- its actual
    //! type, not its data type -- is convertible to the `To` data type with
    //! `static_cast`, that conversion is used. Otherwise, a static assertion
    //! is triggered.
    //!
    //! @note
    //! `convert` is only used to provide the conversions; to actually
    //! perform conversions, use `to`.
    //!
    //! ### Example
    //! @include example/core/convert.cpp
    template <typename To, typename From, typename Enable = void>
    struct convert
        : core_detail::default_convert<To, From>
    { };

    namespace core_detail {
        template <typename To>
        struct to {
            template <typename X>
            constexpr auto operator()(X x) const
            { return convert<To, datatype_t<X>>::apply(x); }
        };
    }

    //! @ingroup core
    //! Create an object of a data type from an object of another data type.
    //!
    //! See `convert` for how to specify user-defined conversions.
    //!
    //! ### Example
    //! @snippet example/list/to.cpp main
    template <typename To>
    constexpr core_detail::to<To> to{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_HPP

#include <boost/hana/bool.hpp>
