/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <typename> struct F;
constexpr auto f = template_<F>;

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto t) {
    return type<F<typename decltype(t)::type>>;
};

struct x0; struct x1; struct x2;

BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<>{}), mpl::vector<>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0>{}), mpl::vector<F<x0>>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0, x1>{}), mpl::vector<F<x0>, F<x1>>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0, x1, x2>{}), mpl::vector<F<x0>, F<x1>, F<x2>>{}));

BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<>{}), mpl::vector<>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0>{}), mpl::vector<F<x0>>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0, x1>{}), mpl::vector<F<x0>, F<x1>>{}));
BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0, x1, x2>{}), mpl::vector<F<x0>, F<x1>, F<x2>>{}));

int main() { (void)g; }
