/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/drop_while.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/tail.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
BOOST_HANA_CONSTANT_CHECK(
    hana::equal(mpl::vector2<int, char>{}, mpl::vector<int, char>{})
);
BOOST_HANA_CONSTANT_CHECK(
    hana::not_equal(mpl::vector2<int, char>{}, mpl::vector<int, char, float>{})
);
//! [Comparable]

}{

//! [Foldable]
auto types = mpl::vector<long, float, short, float, long, long double>{};
auto number_of_floats = hana::fold_left(types, hana::int_c<0>, [](auto count, auto t) {
    return hana::if_(hana::trait<std::is_floating_point>(t),
        count + hana::int_c<1>,
        count
    );
});

BOOST_HANA_CONSTANT_CHECK(number_of_floats == hana::int_c<3>);
//! [Foldable]

}{

//! [Iterable]
BOOST_HANA_CONSTANT_CHECK(hana::front(mpl::vector<int, char, void>{}) == hana::type_c<int>);

BOOST_HANA_CONSTANT_CHECK(hana::equal(
    hana::tail(mpl::vector<int, char, void>{}),
    mpl::vector<char, void>{}
));

BOOST_HANA_CONSTANT_CHECK(hana::equal(
    hana::drop_while(mpl::vector<float, double const, int, float&>{},
                     hana::trait<std::is_floating_point>),
    mpl::vector<int, float&>{}
));
//! [Iterable]

}{

//! [Searchable]
BOOST_HANA_CONSTANT_CHECK(
    hana::find_if(mpl::vector<int, float, char const*>{}, hana::equal.to(hana::type_c<float>))
    ==
    hana::just(hana::type_c<float>)
);

BOOST_HANA_CONSTANT_CHECK(
    hana::find(mpl::vector<int, float, char const*>{}, hana::type_c<void>)
    ==
    hana::nothing
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [from_Foldable]
auto xs = hana::make_tuple(1, '2', 3.0);
static_assert(std::is_same<
    decltype(hana::to<hana::ext::boost::mpl::vector_tag>(xs)),
    mpl::vector<int, char, double>
>{}, "");

auto ys = hana::make_tuple(1, '2', hana::type_c<void>);
static_assert(std::is_same<
    decltype(hana::to<hana::ext::boost::mpl::vector_tag>(ys)),
    mpl::vector<int, char, void>
>{}, "");
//! [from_Foldable]

}

}
