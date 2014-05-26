/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! @todo Use placeholders instead of partial.
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = list_t<char, int, unsigned, long, unsigned long>;
    BOOST_HANA_STATIC_ASSERT(find(partial(equal, type<unsigned>), types) == just(type<unsigned>));
    BOOST_HANA_STATIC_ASSERT(find(partial(equal, type<void>), types) == nothing);
}
