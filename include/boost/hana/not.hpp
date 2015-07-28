/*!
@file
Defines `boost::hana::not_`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_NOT_HPP
#define BOOST_HANA_NOT_HPP

#include <boost/hana/fwd/not.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename X>
    constexpr decltype(auto) not_t::operator()(X&& x) const {
        using Bool = typename hana::datatype<X>::type;
        using Not = BOOST_HANA_DISPATCH_IF(hana::not_impl<Bool>,
            hana::_models<Logical, Bool>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Logical, Bool>::value,
        "hana::not_(cond) requires 'cond' to be a Logical");
    #endif

        return Not::apply(static_cast<X&&>(x));
    }
    //! @endcond

    template <typename L, bool condition>
    struct not_impl<L, when<condition>> : hana::default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    template <typename L>
    struct not_impl<L, hana::when<std::is_arithmetic<L>::value>> {
        template <typename Cond>
        static constexpr Cond apply(Cond const& cond)
        { return static_cast<Cond>(cond ? false : true); }
    };

    template <typename C>
    struct not_impl<C, hana::when<
        hana::_models<hana::Constant, C>::value &&
        hana::_models<hana::Logical, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        template <typename Cond>
        struct constant_t {
            static constexpr decltype(auto) get()
            { return boost::hana::not_(boost::hana::value<Cond>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename Cond>
        static constexpr auto apply(Cond const&)
        { return hana::to<C>(constant_t<Cond>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_NOT_HPP
