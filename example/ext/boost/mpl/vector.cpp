/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
#include <type_traits>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
BOOST_HANA_CONSTANT_CHECK(
    equal(mpl::vector2<int, char>{}, mpl::vector<int, char>{})
);
BOOST_HANA_CONSTANT_CHECK(
    not_equal(mpl::vector2<int, char>{}, mpl::vector<int, char, float>{})
);
//! [Comparable]

}{

//! [Foldable]
auto types = mpl::vector<long, float, short, float, long, long double>{};
auto number_of_floats = fold.left(types, int_<0>, [](auto count, auto t) {
    return if_(trait<std::is_floating_point>(t),
        count + int_<1>,
        count
    );
});

BOOST_HANA_CONSTANT_CHECK(number_of_floats == int_<3>);
//! [Foldable]

}{

//! [Iterable]
BOOST_HANA_CONSTANT_CHECK(head(mpl::vector<int, char, void>{}) == type<int>);

BOOST_HANA_CONSTANT_CHECK(equal(
    tail(mpl::vector<int, char, void>{}),
    mpl::vector<char, void>{}
));

BOOST_HANA_CONSTANT_CHECK(equal(
    drop_while(mpl::vector<float, double const, int, float&>{},
               trait<std::is_floating_point>),
    mpl::vector<int, float&>{}
));
//! [Iterable]

}{

//! [Searchable]
BOOST_HANA_CONSTANT_CHECK(
    find_if(mpl::vector<int, float, char const*>{}, equal.to(type<float>))
    ==
    just(type<float>)
);

BOOST_HANA_CONSTANT_CHECK(
    find(mpl::vector<int, float, char const*>{}, type<void>)
    ==
    nothing
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [from_Foldable]
auto xs = make_tuple(1, '2', 3.0);
static_assert(std::is_same<
    decltype(to<ext::boost::mpl::Vector>(xs)),
    mpl::vector<int, char, double>
>{}, "");

auto ys = make_tuple(1, '2', type<void>);
static_assert(std::is_same<
    decltype(to<ext::boost::mpl::Vector>(ys)),
    mpl::vector<int, char, void>
>{}, "");
//! [from_Foldable]

}

}
