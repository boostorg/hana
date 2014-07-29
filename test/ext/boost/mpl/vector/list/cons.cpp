/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


struct x0; struct x1; struct x2;

int main() {
    using namespace operators;
    BOOST_HANA_STATIC_ASSERT(cons(type<x0>, mpl::vector<>{}) == mpl::vector<x0>{});
    BOOST_HANA_STATIC_ASSERT(cons(type<x0>, mpl::vector<x1>{}) == mpl::vector<x0, x1>{});
    BOOST_HANA_STATIC_ASSERT(cons(type<x0>, mpl::vector<x1, x2>{}) == mpl::vector<x0, x1, x2>{});
}
