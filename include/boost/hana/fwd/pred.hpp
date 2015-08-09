/*!
@file
Forward declares `boost::hana::pred`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PRED_HPP
#define BOOST_HANA_FWD_PRED_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns the predecessor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @include example/pred.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pred = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct pred_impl : pred_impl<E, when<true>> { };

    struct pred_t {
        template <typename N>
        constexpr decltype(auto) operator()(N&& n) const;
    };

    constexpr pred_t pred{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PRED_HPP
