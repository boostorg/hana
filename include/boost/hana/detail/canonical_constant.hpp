/*!
@file
Defines `boost::hana::detail::CanonicalConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP
#define BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP

namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Data type representing a canonical `Constant`.
    //!
    //! This is an implementation detail used to provide many models for
    //! stuff like `Monoid`, `Group`, etc. To create a `CanonicalConstant`,
    //! simply create an object with a nested `hana::datatype` equal to
    //! the proper specialization of `CanonicalConstant<T>`, and then
    //! also provide a static function named `get()` returning the
    //! `constexpr` value.
    template <typename T>
    struct CanonicalConstant {
        using value_type = T;
    };
}}}


#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/fwd/integral_constant.hpp> // ok, this header includes us


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(detail::CanonicalConstant<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<detail::CanonicalConstant<T>> {
        template <typename X>
        static constexpr decltype(auto) apply(X)
        { return X::get(); }
    };

    template <typename T, typename C>
    struct to_impl<detail::CanonicalConstant<T>, C, when<
        models<Constant(C)>{} && is_convertible<typename C::value_type, T>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        template <typename X>
        struct _constant {
            static constexpr decltype(auto) get()
            { return to<T>(boost::hana::value(X{})); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X>
        static constexpr _constant<X> apply(X const&)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralConstant (temporary, until we remove it altogether)
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IntegralConstant::instance<detail::CanonicalConstant<T>, when<
        detail::std::is_integral<T>{}
    >> {
        template <T v>
        struct _constant {
            static constexpr T get() { return v; };
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };

        template <T v>
        static constexpr auto integral_constant_impl()
        { return _constant<v>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP
