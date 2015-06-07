/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/mpl/quote.hpp>

#include <type_traits>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;
using namespace boost::hana;


int main() {

{

//! [hana]
auto tuple = make_tuple(1, 'x', 3.4f);

auto result = find_if(tuple, [](auto const& x) {
  return traits::is_integral(x);
});
//! [hana]
(void)result;

#if 0
//! [hana-explicit]
some_type result = find_if(tuple, [](auto const& x) {
  return traits::is_integral(x);
});
//! [hana-explicit]
#endif

}{

//! [fusion]
using Container = fusion::result_of::make_vector<int, char, float>::type;
Container tuple = fusion::make_vector(1, 'x', 3.4f);

using Predicate = mpl::quote1<std::is_integral>;
using Result = fusion::result_of::find_if<Container, Predicate>::type;
Result result = fusion::find_if<Predicate>(tuple);
//! [fusion]
(void)result;

}

}
