/*!
@file
Defines `boost::hana::zero`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ZERO_HPP
#define BOOST_HANA_ZERO_HPP

#include <boost/hana/fwd/zero.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/canonical_constant.hpp>

#include <type_traits>


namespace boost { namespace hana {
    template <typename M>
    struct zero_t {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monoid, M>::value,
        "hana::zero<M>() requires 'M' to be a Monoid");
    #endif

        constexpr decltype(auto) operator()() const {
            using Zero = BOOST_HANA_DISPATCH_IF(zero_impl<M>,
                _models<Monoid, M>::value
            );

            return Zero::apply();
        }
    };

    template <typename M, bool condition>
    struct zero_impl<M, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct zero_impl<T, when<
        std::is_arithmetic<T>::value &&
        !std::is_same<T, bool>::value
    >> {
        static constexpr T apply()
        { return static_cast<T>(0); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Monoid
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct zero_impl<C, when<
        _models<Constant, C>::value &&
        _models<Monoid, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        //! @cond
        struct constant_t {
            static constexpr decltype(auto) get()
            { return boost::hana::zero<T>(); }

            using hana = constant_t;
            using tag = detail::CanonicalConstant<T>;
        };
        //! @endcond
        static constexpr decltype(auto) apply()
        { return hana::to<C>(constant_t{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ZERO_HPP
