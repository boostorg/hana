/*!
@file
Defines `boost::hana::detail::as_container_element_t`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_AS_CONTAINER_ELEMENT_HPP
#define BOOST_HANA_DETAIL_AS_CONTAINER_ELEMENT_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/decay.hpp>


// We forward declare `std::reference_wrapper` because including the whole
// `<functional>` header is too expensive in terms of compile-time.
// Technically, this is disallowed by the standard, and we'll have to handle
// each standard library separately. See this issue for more information:
// https://github.com/ldionne/hana/issues/176.
//
// Dear Standard Committee, it is about time we get fine grained headers...
#if defined(BOOST_HANA_CONFIG_LIBCPP)

    _LIBCPP_BEGIN_NAMESPACE_STD
        template <class _Tp>
        class _LIBCPP_TYPE_VIS_ONLY reference_wrapper;
    _LIBCPP_END_NAMESPACE_STD

#elif defined(BOOST_HANA_CONFIG_LIBSTDCXX)

    namespace std _GLIBCXX_VISIBILITY(default) {
    _GLIBCXX_BEGIN_NAMESPACE_VERSION
        template <class _Tp>
        class reference_wrapper;
    _GLIBCXX_END_NAMESPACE_VERSION
    }

#else

#   error "Hana must be taught how to forward declare std::reference_wrapper for your standard library"

#endif


// We also forward declare boost::reference_wrapper, because it's nice to
// interoperate with it too.
namespace boost { template <class T> class reference_wrapper; }


namespace boost { namespace hana { namespace detail {
    template <typename T>
    struct as_container_element_impl {
        using type = T;
    };

    template <typename T>
    struct as_container_element_impl<std::reference_wrapper<T>> {
        using type = T&;
    };

    template <typename T>
    struct as_container_element_impl<boost::reference_wrapper<T>> {
        using type = T&;
    };

    template <typename T>
    using as_container_element_t = typename as_container_element_impl<
        typename detail::decay<T>::type
    >::type;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_AS_CONTAINER_ELEMENT_HPP
