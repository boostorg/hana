/*!
@file
Defines `boost::hana::succ`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SUCC_HPP
#define BOOST_HANA_SUCC_HPP

#include <boost/hana/fwd/succ.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/enumerable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename N>
    constexpr decltype(auto) succ_t::operator()(N&& n) const {
        using E = typename hana::tag_of<N>::type;
        using Succ = BOOST_HANA_DISPATCH_IF(succ_impl<E>,
            Enumerable<E>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Enumerable<E>::value,
        "hana::succ(n) requires 'n' to be Enumerable");
    #endif

        return Succ::apply(static_cast<N&&>(n));
    }
    //! @endcond

    template <typename E, bool condition>
    struct succ_impl<E, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename E>
    struct succ_impl<E, when<
        std::is_arithmetic<E>::value && !std::is_same<E, bool>::value
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return ++x; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over an Enumerable data type
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename C, typename X>
        struct constant_from_succ {
            static constexpr auto value = hana::succ(hana::value<X>());
            using hana_tag = detail::CanonicalConstant<typename C::value_type>;
        };
    }

    template <typename C>
    struct succ_impl<C, when<
        Constant<C>::value &&
        Enumerable<typename C::value_type>::value
    >> {
        template <typename X>
        static constexpr decltype(auto) apply(X const&)
        { return hana::to<C>(detail::constant_from_succ<C, X>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SUCC_HPP
