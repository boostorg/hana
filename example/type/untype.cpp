/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [main]
    struct T;
    BOOST_HANA_CONSTEXPR_LAMBDA auto t = type<T>;
    BOOST_HANA_STATIC_ASSERT(std::is_same<untype_t<decltype(t)>, T>{});
    //! [main]
}
