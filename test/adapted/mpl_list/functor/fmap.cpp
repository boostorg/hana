/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/mpl_list.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <typename>
struct f;

struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(equal(fmap(template_<f>, mpl::list<>{}), mpl::list<>{}));
    BOOST_HANA_STATIC_ASSERT(equal(fmap(template_<f>, mpl::list<x0>{}), mpl::list<f<x0>>{}));
    BOOST_HANA_STATIC_ASSERT(equal(fmap(template_<f>, mpl::list<x0, x1>{}), mpl::list<f<x0>, f<x1>>{}));
    BOOST_HANA_STATIC_ASSERT(equal(fmap(template_<f>, mpl::list<x0, x1, x2>{}), mpl::list<f<x0>, f<x1>, f<x2>>{}));
}
