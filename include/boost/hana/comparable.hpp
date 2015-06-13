/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/fwd/comparable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/concepts.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(equal)>{}() ||
            _has_operator<datatype_t<Y>, decltype(equal)>{}()
        >>
        constexpr decltype(auto) operator==(X&& x, Y&& y)
        { return hana::equal(static_cast<X&&>(x), static_cast<Y&&>(y)); }

        template <typename X, typename Y, typename = std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(not_equal)>{}() ||
            _has_operator<datatype_t<Y>, decltype(not_equal)>{}()
        >>
        constexpr decltype(auto) operator!=(X&& x, Y&& y)
        { return hana::not_equal(static_cast<X&&>(x), static_cast<Y&&>(y)); }
    }

    //////////////////////////////////////////////////////////////////////////
    // equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct equal_impl : equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&&, Y&&) {
            using T_ = detail::dependent_on_t<sizeof(X) == 1, T>;
            static_assert(!is_convertible<T_, U>{} && !is_convertible<U, T_>{},
            "No default implementation of hana::equal is provided for related "
            "types that can't be safely embedded into a common type, because "
            "those are most likely programming errors. If this is really what "
            "you want, you can manually convert both objects to a common "
            "Comparable type before performing the comparison.");

            return false_;
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Comparable, T, U>{}() &&
        !detail::EqualityComparable<T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::equal(hana::to<C>(static_cast<X&&>(x)),
                               hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _equal_to::operator()(X&& x) const
    { return hana::partial(equal, static_cast<X&&>(x)); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // not_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct not_equal_impl : not_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct not_equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_(hana::equal(static_cast<X&&>(x),
                                          static_cast<Y&&>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct not_equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Comparable, T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_equal(hana::to<C>(static_cast<X&&>(x)),
                                   hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _not_equal_to::operator()(X&& x) const
    { return hana::partial(not_equal, static_cast<X&&>(x)); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // comparing
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct _comparing {
        F f;

        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const&
        { return hana::equal(f(static_cast<X&&>(x)), f(static_cast<Y&&>(y))); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) &
        { return hana::equal(f(static_cast<X&&>(x)), f(static_cast<Y&&>(y))); }
#endif
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models_impl<Comparable, T>
        : _integral_constant<bool,
            !is_default<equal_impl<T, T>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for EqualityComparable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct equal_impl<T, U, when<detail::EqualityComparable<T, U>{}()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) == static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants wrapping a Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct equal_impl<C, C, when<
        _models<Constant, C>{}() &&
        _models<Comparable, typename C::value_type>{}()
    >> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&) {
            constexpr auto equal = hana::equal(hana::value<X>(), hana::value<Y>());
            constexpr bool truth_value = hana::if_(equal, true, false);
            return bool_<truth_value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
