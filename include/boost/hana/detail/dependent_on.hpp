/*!
@file
Defines `boost::hana::detail::dependent_on`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_DEPENDENT_ON_HPP
#define BOOST_HANA_DETAIL_DEPENDENT_ON_HPP

namespace boost { namespace hana { namespace detail {
    template <typename, typename T>
    struct dependent_on_impl { using type = T; };

    template <typename Dummy, typename T>
    using dependent_on = typename dependent_on_impl<Dummy, T>::type;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_DEPENDENT_ON_HPP
