/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functor/laws.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <typename> struct f_;
template <typename> struct g_;
BOOST_HANA_CONSTEXPR_LAMBDA auto f = template_<f_>;
BOOST_HANA_CONSTEXPR_LAMBDA auto g = template_<g_>;

struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(mpl::list<>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(mpl::list<x0>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(mpl::list<x0, x1>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(mpl::list<x0, x1, x2>{}, f, g));
}
