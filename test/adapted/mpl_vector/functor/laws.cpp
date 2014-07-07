/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/mpl_vector.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <typename> struct f_;
template <typename> struct g_;
BOOST_HANA_CONSTEXPR_LAMBDA auto f = template_<f_>;
BOOST_HANA_CONSTEXPR_LAMBDA auto g = template_<g_>;

struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(mpl::vector<>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(mpl::vector<x0>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(mpl::vector<x0, x1>{}, f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(mpl::vector<x0, x1, x2>{}, f, g));
}
