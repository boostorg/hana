/*!
@file
Defines `boost::hana::instantiates`, `boost::hana::is_a` and `boost::hana::is_an`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_INSTANTIATES_HPP
#define BOOST_HANA_CORE_INSTANTIATES_HPP

#include <boost/hana/core/typeclass.hpp> // for disable
#include <boost/hana/detail/bool_fwd.hpp>


namespace boost { namespace hana {
    namespace core_detail {
        constexpr auto instantiates_impl(...) { return true_; }
        constexpr auto instantiates_impl(disable*) { return false_; }
    }

    //! @ingroup group-core
    //! Variable template representing whether the type class is instantiated
    //! with the given arguments.
    //!
    //! Specifically, `instantiates<Typeclass, Datatypes...>` is a
    //! [compile-time](@ref Logical_terminology) `Logical` representing
    //! whether `Typeclass::instance<Datatypes...>` represents an instance
    //! of `Typeclass`. This does not check whether the instance is valid:
    //! it does not check whether laws are satisfied or even if the minimal
    //! complete definition is syntactically correct. It only tells whether
    //! an instance is there and was not disabled explicitly with `disable`.
    //!
    //! This is provided in addition to `is_a` for type classes taking more
    //! than one argument or when no object of the data type is available.
    //!
    //! ### Example
    //! @snippet example/core/instantiates.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto instantiates = unspecified;
#else
    template <typename Typeclass, typename ...Datatypes>
    constexpr auto instantiates = core_detail::instantiates_impl(
        (typename Typeclass::template instance<Datatypes...>*)0
    );
#endif

    namespace core_detail {
        template <typename Typeclass>
        struct is_a {
            template <typename X>
            constexpr auto operator()(X x) const
            { return instantiates<Typeclass, datatype_t<X>>; }
        };
    }

    //! @ingroup group-core
    //! Return whether an object is an instance of the given type class.
    //!
    //! ### Example
    //! @snippet example/core/is_a.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Typeclass>
    constexpr auto is_a = [](auto x) {
        return instantiates<Typeclass, datatype_t<decltype(x)>>;
    };
#else
    template <typename Typeclass>
    constexpr core_detail::is_a<Typeclass> is_a{};
#endif

    //! @ingroup group-core
    //! Equivalent to `is_a`; provided for consistency with the rules of the
    //! English language.
    template <typename Typeclass>
    constexpr auto is_an = is_a<Typeclass>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_INSTANTIATES_HPP

#include <boost/hana/bool.hpp>
