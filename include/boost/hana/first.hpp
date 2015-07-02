/*!
@file
Defines `boost::hana::first`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FIRST_HPP
#define BOOST_HANA_FIRST_HPP

#include <boost/hana/fwd/first.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>


namespace boost { namespace hana {
    struct Product; //! @todo include the forward declaration instead

    //! @cond
    template <typename Pair>
    constexpr decltype(auto) first_t::operator()(Pair&& pair) const {
        using P = typename datatype<Pair>::type;
        using First = BOOST_HANA_DISPATCH_IF(first_impl<P>,
            _models<Product, P>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Product, P>::value,
        "hana::first(pair) requires 'pair' to be a Product");
    #endif

        return First::apply(static_cast<Pair&&>(pair));
    }
    //! @endcond

    template <typename P, bool condition>
    struct first_impl<P, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FIRST_HPP
