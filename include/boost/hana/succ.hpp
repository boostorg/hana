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

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename N>
    constexpr decltype(auto) succ_t::operator()(N&& n) const {
        using E = typename datatype<N>::type;
        using Succ = BOOST_HANA_DISPATCH_IF(succ_impl<E>,
            _models<Enumerable, E>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Enumerable, E>::value,
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
    template <typename C>
    struct succ_impl<C, when<
        _models<Constant, C>::value &&
        _models<Enumerable, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        template <typename X>
        struct constant_t {
            static constexpr decltype(auto) get()
            { return boost::hana::succ(boost::hana::value<X>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X>
        static constexpr decltype(auto) apply(X const&)
        { return hana::to<C>(constant_t<X>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SUCC_HPP
