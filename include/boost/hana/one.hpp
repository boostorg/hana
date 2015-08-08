/*!
@file
Defines `boost::hana::one`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ONE_HPP
#define BOOST_HANA_ONE_HPP

#include <boost/hana/fwd/one.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/canonical_constant.hpp>

#include <type_traits>


namespace boost { namespace hana {
    template <typename R>
    struct one_t {

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Ring, R>::value,
        "hana::one<R>() requires 'R' to be a Ring");
    #endif

        constexpr decltype(auto) operator()() const {
            using One = BOOST_HANA_DISPATCH_IF(one_impl<R>,
                _models<Ring, R>::value
            );

            return One::apply();
        }
    };

    template <typename R, bool condition>
    struct one_impl<R, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct one_impl<T, when<std::is_arithmetic<T>::value &&
                            !std::is_same<bool, T>::value>> {
        static constexpr T apply()
        { return static_cast<T>(1); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Ring
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct one_impl<C, when<
        _models<Constant, C>::value &&
        _models<Ring, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        //! @cond
        struct constant_t {
            static constexpr decltype(auto) get()
            { return boost::hana::one<T>(); }

            using hana = constant_t;
            using datatype = detail::CanonicalConstant<T>;
        };
        //! @endcond
        static constexpr decltype(auto) apply()
        { return hana::to<C>(constant_t{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ONE_HPP
