/*!
@file
Defines `boost::hana::to` and related utilities.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_CONVERT_HPP
#define BOOST_HANA_CORE_CONVERT_HPP

#include <boost/hana/fwd/core/convert.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    //////////////////////////////////////////////////////////////////////////
    // to
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename To, typename From, typename>
    struct to_impl : to_impl<To, From, when<true>> { };
    //! @endcond

    namespace convert_detail {
        struct no_conversion { };

        template <typename ...>
        struct is_valid { static constexpr bool value = true; };
    }

    template <typename To, typename From, bool condition>
    struct to_impl<To, From, when<condition>> : convert_detail::no_conversion {
        template <typename X>
        static constexpr auto apply(X const&) {
            static_assert(detail::wrong<to_impl<To, From>, X>{},
            "no conversion is available between the provided types");
        }
    };

    template <typename To, typename From>
    struct to_impl<To, From, when<convert_detail::is_valid<
        decltype(static_cast<To>(std::declval<From>()))
    >::value>> {
        template <typename X>
        static constexpr To apply(X&& x)
        { return static_cast<To>(static_cast<X&&>(x)); }
    };

    template <typename To>
    struct to_impl<To, To> : embedding<> {
        template <typename X>
        static constexpr X apply(X&& x)
        { return static_cast<X&&>(x); }
    };

    //! @cond
    template <typename To>
    template <typename X>
    constexpr decltype(auto) to_t<To>::operator()(X&& x) const {
        using From = typename hana::tag_of<X>::type;
        return to_impl<To, From>::apply(static_cast<X&&>(x));
    }
    //! @endcond

#define BOOST_HANA_DEFINE_EMBEDDING_IMPL(TO, FROM)                          \
    template <>                                                             \
    struct to_impl<TO, FROM> : embedding<>                                  \
    { static constexpr TO apply(FROM x) { return x; } }                     \
/**/
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long double, double);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long double, float);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(double     , float);

    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long, long);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long, int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long, short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long, signed char);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long     , int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long     , short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long     , signed char);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(int      , short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(int      , signed char);

    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long, unsigned long);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long, unsigned int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long, unsigned short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long, unsigned char);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long     , unsigned int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long     , unsigned short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long     , unsigned char);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned int      , unsigned short);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned int      , unsigned char);

    template <typename T>
    struct to_impl<T*, decltype(nullptr)> : embedding<> {
        static constexpr T* apply(decltype(nullptr)) { return nullptr; }
    };
#undef BOOST_HANA_DEFINE_EMBEDDING_IMPL

    //////////////////////////////////////////////////////////////////////////
    // is_convertible
    //////////////////////////////////////////////////////////////////////////
    template <typename From, typename To, typename>
    struct is_convertible : std::true_type { };

    template <typename From, typename To>
    struct is_convertible<From, To, decltype((void)
        static_cast<convert_detail::no_conversion>(*(to_impl<To, From>*)0)
    )> : std::false_type { };

    //////////////////////////////////////////////////////////////////////////
    // is_embedded
    //////////////////////////////////////////////////////////////////////////
    template <typename From, typename To, typename>
    struct is_embedded : std::false_type { };

    template <typename From, typename To>
    struct is_embedded<From, To, decltype((void)
        static_cast<embedding<true>>(*(to_impl<To, From>*)0)
    )> : std::true_type { };

    //////////////////////////////////////////////////////////////////////////
    // Conversion for Constants
    //////////////////////////////////////////////////////////////////////////
    template <typename To, typename From>
    struct to_impl<To, From, when<
        Constant<From>::value &&
        is_convertible<typename From::value_type, To>::value
    >> : embedding<is_embedded<typename From::value_type, To>::value> {
        template <typename X>
        static constexpr decltype(auto) apply(X const&)
        { return hana::to<To>(hana::value<X>()); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable -> Sequence
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename F>
    struct to_impl<S, F, when<
        Sequence<S>::value &&
        Foldable<F>::value
    >> : embedding<Sequence<F>::value> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::unpack(static_cast<Xs&&>(xs), hana::make<S>); }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CORE_CONVERT_HPP
