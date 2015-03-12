/*!
@file
Defines `boost::hana::Enumerable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ENUMERABLE_HPP
#define BOOST_HANA_ENUMERABLE_HPP

#include <boost/hana/fwd/enumerable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/detail/std/is_same.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // succ
    //////////////////////////////////////////////////////////////////////////
    template <typename E, typename>
    struct succ_impl : succ_impl<E, when<true>> { };

    template <typename E, bool condition>
    struct succ_impl<E, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // pred
    //////////////////////////////////////////////////////////////////////////
    template <typename E, typename>
    struct pred_impl : pred_impl<E, when<true>> { };

    template <typename E, bool condition>
    struct pred_impl<E, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename E>
    struct models_impl<Enumerable, E>
        : _integral_constant<bool,
            !is_default<succ_impl<E>>{} &&
            !is_default<pred_impl<E>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename E>
    struct succ_impl<E, when<
        detail::std::is_arithmetic<E>{} && !detail::std::is_same<E, bool>{}
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return ++x; }
    };

    template <typename E>
    struct pred_impl<E, when<
        detail::std::is_arithmetic<E>{} && !detail::std::is_same<E, bool>{}
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return --x; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over an Enumerable data type
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct succ_impl<C, when<
        _models<Constant, C>{} && _models<Enumerable, typename C::value_type>{}
    >> {
        using T = typename C::value_type;
        template <typename X>
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::succ(boost::hana::value<X>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X>
        static constexpr decltype(auto) apply(X const&)
        { return to<C>(_constant<X>{}); }
    };

    template <typename C>
    struct pred_impl<C, when<
        _models<Constant, C>{} && _models<Enumerable, typename C::value_type>{}
    >> {
        using T = typename C::value_type;
        template <typename X>
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::pred(boost::hana::value<X>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X>
        static constexpr decltype(auto) apply(X const&)
        { return to<C>(_constant<X>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ENUMERABLE_HPP
