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

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // to
    //////////////////////////////////////////////////////////////////////////
    template <typename To, typename From, typename>
    struct to_impl : to_impl<To, From, when<true>> { };

    namespace core_detail { struct no_conversion { }; }

    template <typename To, typename From, bool condition>
    struct to_impl<To, From, when<condition>> : core_detail::no_conversion {
        template <typename X>
        static constexpr auto apply(X const&) {
            static_assert(wrong<to_impl<To, From>, X>{},
            "no conversion is available between the provided data types");
        }
    };

    template <typename To, typename From>
    struct to_impl<To, From, when_valid<
        decltype(static_cast<To>(detail::std::declval<From>()))
    >> {
        template <typename X>
        static constexpr To apply(X&& x)
        { return static_cast<To>(detail::std::forward<X>(x)); }
    };

    template <typename To>
    struct to_impl<To, To> : embedding<> {
        template <typename X>
        static constexpr X apply(X&& x)
        { return detail::std::forward<X>(x); }
    };

    template <typename To>
    struct _to {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const {
            return to_impl<To, typename datatype<X>::type>::apply(
                                                detail::std::forward<X>(x));
        }
    };

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
    struct is_convertible : detail::std::true_type { };

    template <typename From, typename To>
    struct is_convertible<From, To, decltype((void)
        static_cast<core_detail::no_conversion>(*(to_impl<To, From>*)0)
    )> : detail::std::false_type { };

    //////////////////////////////////////////////////////////////////////////
    // is_embedded
    //////////////////////////////////////////////////////////////////////////
    template <typename From, typename To, typename>
    struct is_embedded : detail::std::false_type { };

    template <typename From, typename To>
    struct is_embedded<From, To, decltype((void)
        static_cast<embedding<true>>(*(to_impl<To, From>*)0)
    )> : detail::std::true_type { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_CONVERT_HPP
