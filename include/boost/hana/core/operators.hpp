/*!
@file
Defines `boost::hana::operators` and related utilities.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_OPERATORS_HPP
#define BOOST_HANA_CORE_OPERATORS_HPP

#include <boost/hana/fwd/core/operators.hpp>

#include <boost/hana/fwd/bool.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // operators::of
    //////////////////////////////////////////////////////////////////////////
    namespace core_detail {
        template <typename Concept, typename = void>
        struct check_nested {
            using type = check_nested;
        };

        template <typename Concept>
        struct check_nested<Concept, decltype((void)(
            typename Concept::hana::operators{}
        ))> {
            using type = typename Concept::hana::operators;
        };
    }

    namespace operators {
        template <>
        struct of<> { };

        template <typename Concept>
        struct of<Concept> : core_detail::check_nested<Concept>::type { };

        template <typename Concept, typename ...Rest>
        struct of<Concept, Rest...> : of<Concept>, of<Rest>... { };
    }

    //////////////////////////////////////////////////////////////////////////
    // has_operator
    //////////////////////////////////////////////////////////////////////////
    template <typename Datatype, typename Method, typename>
    struct _has_operator
        : _integral_constant<bool, false>
    { };

    template <typename Datatype, typename Method>
    struct _has_operator<Datatype, Method, decltype((void)(
        static_cast<Method>(operators::of<Datatype>{})
    ))>
        : _integral_constant<bool, true>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_OPERATORS_HPP

#include <boost/hana/bool.hpp>
