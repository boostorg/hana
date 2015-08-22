/*!
@file
Defines `boost::hana::detail::CanonicalConstant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP
#define BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP

namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Tag representing a canonical `Constant`.
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


#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<detail::CanonicalConstant<T>> {
        template <typename X>
        static constexpr decltype(auto) apply()
        { return X::get(); }
    };

    template <typename T, typename C>
    struct to_impl<detail::CanonicalConstant<T>, C, when<
        _models<Constant, C>::value &&
        is_convertible<typename C::value_type, T>::value
    >>
        : embedding<is_embedded<typename C::value_type, T>::value>
    {
        template <typename X>
        struct constant_t {
            static constexpr decltype(auto) get()
            { return to<T>(boost::hana::value<X>()); }

            using hana = constant_t;
            using datatype = detail::CanonicalConstant<T>;
        };
        template <typename X>
        static constexpr constant_t<X> apply(X const&)
        { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_CANONICAL_CONSTANT_HPP
