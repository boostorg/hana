/*!
@file
Defines `boost::hana::convert` and `boost::hana::to`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_CONVERT_HPP
#define BOOST_HANA_CORE_CONVERT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
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

    //! @ingroup group-core
    //! Implements conversions between data types.
    //!
    //! To specify a conversion between two data types, one must specialize
    //! `convert` for the corresponding data types. `when` can be used to
    //! specify conversions between all data types satisfying some predicate.
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
    template <typename To, typename From, typename = void>
    struct convert
    //! @cond
        : convert<To, From, when<true>>
    //! @endcond
    { };

    template <typename To, typename From, bool condition>
    struct convert<To, From, when<condition>>
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

    //! @ingroup group-core
    //! Create an object of a data type from an object of another data type.
    //!
    //! See `convert` to specify _how_ to convert from a data type to another.
    //!
    //!
    //! @tparam To
    //! The data type to which `x` should be converted.
    //!
    //! @param x
    //! The object to convert to the given data type.
    //!
    //!
    //! ### Example
    //! @snippet example/core/to.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename To>
    constexpr auto to = [](auto x) {
        return convert<To, datatype_t<decltype(x)>>::apply(x);
    };
#else
    template <typename To>
    constexpr core_detail::to<To> to{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_CONVERT_HPP
