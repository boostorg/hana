/*!
@file
Defines `boost::hana::mult`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MULT_HPP
#define BOOST_HANA_MULT_HPP

#include <boost/hana/fwd/mult.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) mult_t::operator()(X&& x, Y&& y) const {
        using T = typename datatype<X>::type;
        using U = typename datatype<Y>::type;
        using Mult = BOOST_HANA_DISPATCH_IF(decltype(mult_impl<T, U>{}),
            _models<Ring, T>::value &&
            _models<Ring, U>::value &&
            !is_default<mult_impl<T, U>>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Ring, T>::value,
        "hana::mult(x, y) requires 'x' to be in a Ring");

        static_assert(_models<Ring, U>::value,
        "hana::mult(x, y) requires 'y' to be in a Ring");

        static_assert(!is_default<mult_impl<T, U>>::value,
        "hana::mult(x, y) requires 'x' and 'y' to be embeddable "
        "in a common Ring");
    #endif

        return Mult::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }
    //! @endcond

    template <typename T, typename U, bool condition>
    struct mult_impl<T, U, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    // Cross-type overload
    template <typename T, typename U>
    struct mult_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Ring, T, U>::value
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::mult(hana::to<C>(static_cast<X&&>(x)),
                              hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct mult_impl<T, T, when<std::is_arithmetic<T>::value &&
                                !std::is_same<bool, T>::value>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) * static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Ring
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct mult_impl<C, C, when<
        _models<Constant, C>::value &&
        _models<Ring, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        //! @cond
        template <typename X, typename Y>
        struct constant_t {
            static constexpr decltype(auto) get() {
                return boost::hana::mult(boost::hana::value<X>(),
                                         boost::hana::value<Y>());
            }

            using hana = constant_t;
            using datatype = detail::CanonicalConstant<T>;
        };
        //! @endcond
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return hana::to<C>(constant_t<X, Y>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MULT_HPP
