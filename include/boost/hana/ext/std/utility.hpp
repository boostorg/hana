/*!
@file
Defines adapters for components in the `<utility>` header.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_UTILITY_HPP
#define BOOST_HANA_EXT_STD_UTILITY_HPP

#include <boost/hana/ext/std/integer_sequence.hpp>
#include <boost/hana/ext/std/pair.hpp>

#include <boost/hana/detail/std/add_rvalue_reference.hpp>


namespace boost { namespace hana { namespace traits {
    struct _declval {
        template <typename T>
        typename detail::std::add_rvalue_reference<
            typename T::type
        >::type operator()(T const&) const;
    };

    constexpr _declval declval{};
}}} // end namespace boost::hana::traits

#endif // !BOOST_HANA_EXT_STD_UTILITY_HPP
