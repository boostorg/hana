/*!
@file
Defines `boost::hana::detail::operators::adl`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_OPERATORS_ADL_HPP
#define BOOST_HANA_DETAIL_OPERATORS_ADL_HPP

namespace boost { namespace hana { namespace detail { namespace operators {
    //! @ingroup group-details
    //! Enables [ADL](http://en.cppreference.com/w/cpp/language/adl) in the
    //! `hana::detail::operators` namespace.
    //!
    //! This is used by containers in Hana as a quick way to automatically
    //! define the operators associated to some concepts, in conjunction
    //! with the `detail::xxx_operators` family of metafunctions.
    struct adl { };
}}}} // end namespace boost::hana::detail::operators

#endif // !BOOST_HANA_DETAIL_OPERATORS_ADL_HPP
