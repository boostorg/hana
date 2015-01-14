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
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace core_detail {
        template <typename To, typename From>
        struct default_convert {
            template <typename X>
            static constexpr auto apply_impl(X&& x, int)
                -> decltype(static_cast<To>(detail::std::forward<X>(x)))
            { return static_cast<To>(detail::std::forward<X>(x)); }

            template <typename X>
            static constexpr auto apply_impl(X const&, ...) {
                static_assert((sizeof(X), false),
                "there exists no conversion between the given data types");
            }

            template <typename X>
            static constexpr decltype(auto) apply(X&& x)
            { return apply_impl(detail::std::forward<X>(x), int{}); }
        };

        template <typename To>
        struct default_convert<To, To> {
            template <typename X>
            static constexpr X apply(X&& x) {
                return detail::std::forward<X>(x);
            }
        };
    }

    //! @ingroup group-core
    //! Implements conversions between data types.
    //!
    //! To specify a conversion between two data types, one must specialize
    //! `convert` in the `boost::hana` namespace for the corresponding data
    //! types. `when` can be used to perform flexible specialization.
    //!
    //! By default, `convert` has the following behavior:
    //! If the `To` and `From` data types are the same, the object is
    //! forwarded as-is. Otherwise, if the type of the converted-from object
    //! -- its actual type, not its hana data type -- is convertible to the
    //! `To` data type with `static_cast`, that conversion is used. Otherwise,
    //! calling `convert<To, From>::%apply` triggers a static assertion.
    //!
    //! @note
    //! `convert` is only used to provide the conversions; to actually
    //! perform conversions, use `to`.
    //!
    //! @todo
    //! Use more clever dispatching for this so it is possible to ask whether
    //! a conversion exists, and then use this to implement `common`.
    //!
    //! ### Example
    //! @include example/core/convert.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename To, typename From, typename optional when-based enabler>
    struct convert { };
#else
    template <typename To, typename From, typename = void>
    struct convert
        : convert<To, From, when<true>>
    { };

    template <typename To, typename From, bool condition>
    struct convert<To, From, when<condition>>
        : core_detail::default_convert<To, From>
    { };
#endif

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
    constexpr auto to = [](auto&& x) -> decltype(auto) {
        return convert<To, datatype_t<decltype(x)>>::apply(
            std::forward<decltype(x)>(x)
        );
    };
#else
    template <typename To>
    struct _to {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return convert<To, typename datatype<X>::type>::apply(
                detail::std::forward<X>(x)
            );
        }
    };

    template <typename To>
    constexpr _to<To> to{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_CONVERT_HPP
