/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <string>
#include <type_traits>
#include <vector>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;
using namespace boost::hana::literals;
using namespace boost::hana;


template <int n>
struct storage { char weight[n]; };

int main() {

{

//! [tuple]
auto types = make_tuple(type<int*>, type<char&>, type<void>);
auto char_ref = types[1_c];

BOOST_HANA_CONSTANT_CHECK(char_ref == type<char&>);
//! [tuple]

}{

//! [filter.MPL]
using types = mpl::vector<int, char&, void*>;
using ts = mpl::copy_if<types, mpl::or_<std::is_pointer<mpl::_1>,
                                        std::is_reference<mpl::_1>>>::type;
//                             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//                             placeholder expression

static_assert(mpl::equal<ts, mpl::vector<char&, void*>>::value, "");
//! [filter.MPL]

}{

using traits::is_pointer;
using traits::is_reference;

//! [filter.Hana]
auto types = tuple_t<int*, char&, void>;

auto ts = filter(types, [](auto t) {
  return or_(is_pointer(t), is_reference(t));
});

BOOST_HANA_CONSTANT_CHECK(ts == tuple_t<int*, char&>);
//! [filter.Hana]

}{

//! [single_library.then]
// types (MPL)
using types = mpl::vector<int*, char&, void>;
using ts = mpl::copy_if<types, mpl::or_<std::is_pointer<mpl::_1>,
                                        std::is_reference<mpl::_1>>>::type;

// values (Fusion)
auto values = fusion::make_vector(1, 'c', nullptr, 3.5);
auto vs = fusion::filter_if<std::is_integral<mpl::_1>>(values);
//! [single_library.then]

static_assert(mpl::equal<ts, mpl::vector<int*, char&>>::value, "");
BOOST_HANA_RUNTIME_CHECK(vs == fusion::make_vector(1, 'c'));

}{

using traits::is_pointer;
using traits::is_reference;
using traits::is_integral;

//! [single_library.Hana]
// types
auto types = tuple_t<int*, char&, void>;
auto ts = filter(types, [](auto t) {
  return or_(is_pointer(t), is_reference(t));
});

// values
auto values = make_tuple(1, 'c', nullptr, 3.5);
auto vs = filter(values, [](auto t) {
  return is_integral(t);
});
//! [single_library.Hana]

BOOST_HANA_CONSTANT_CHECK(ts == tuple_t<int*, char&>);
BOOST_HANA_RUNTIME_CHECK(vs == make_tuple(1, 'c'));

}{

//! [make_map.Fusion]
auto map = fusion::make_map<char, int, long, float, double, void>(
  "char", "int", "long", "float", "double", "void"
);

std::string Int = fusion::at_key<int>(map);
BOOST_HANA_RUNTIME_CHECK(Int == "int");
//! [make_map.Fusion]

}{

//! [make_map.Hana]
auto map = make_map(
  make_pair(type<char>,   "char"),
  make_pair(type<int>,    "int"),
  make_pair(type<long>,   "long"),
  make_pair(type<float>,  "float"),
  make_pair(type<double>, "double")
);

std::string Int = map[type<int>];
BOOST_HANA_RUNTIME_CHECK(Int == "int");
//! [make_map.Hana]

}{

using traits::add_pointer;

//! [skip_first_step]
auto types = tuple_t<int*, char&, void>; // first step skipped

auto pointers = transform(types, [](auto t) {
  return add_pointer(t);
});
//! [skip_first_step]

BOOST_HANA_CONSTANT_CHECK(pointers == tuple_t<int**, char*, void*>);

}{

//! [traits]
BOOST_HANA_CONSTANT_CHECK(traits::add_pointer(type<int>) == type<int*>);
BOOST_HANA_CONSTANT_CHECK(traits::common_type(type<int>, type<long>) == type<long>);
BOOST_HANA_CONSTANT_CHECK(traits::is_integral(type<int>));

auto types = tuple_t<int, char, long>;
BOOST_HANA_CONSTANT_CHECK(all_of(types, traits::is_integral));
//! [traits]

}

}

namespace mpl_based {
using namespace mpl;

//! [smallest.MPL]
template <typename ...T>
struct smallest
  : deref<
    typename min_element<
      vector<T...>, less<sizeof_<_1>, sizeof_<_2>>
    >::type
  >
{ };

template <typename ...T>
using smallest_t = typename smallest<T...>::type;

static_assert(std::is_same<
  smallest_t<char, long, long double>,
  char
>::value, "");
//! [smallest.MPL]

static_assert(std::is_same<
  smallest_t<storage<3>, storage<1>, storage<2>>,
  storage<1>
>::value, "");
} // end namespace mpl_based

namespace hana_based {
//! [smallest.Hana]
template <typename ...T>
auto smallest = minimum(make_tuple(type<T>...), [](auto t, auto u) {
  return sizeof_(t) < sizeof_(u);
});

template <typename ...T>
using smallest_t = typename decltype(smallest<T...>)::type;

static_assert(std::is_same<
  smallest_t<char, long, long double>, char
>::value, "");
//! [smallest.Hana]

static_assert(std::is_same<
  smallest_t<storage<3>, storage<1>, storage<2>>,
  storage<1>
>::value, "");
} // end namespace hana_based


namespace metafunction1 {
//! [metafunction1]
template <template <typename> class F, typename T>
constexpr auto metafunction(_type<T> const&)
{ return type<typename F<T>::type>; }

auto t = type<int>;
BOOST_HANA_CONSTANT_CHECK(metafunction<std::add_pointer>(t) == type<int*>);
//! [metafunction1]
}

namespace metafunction2 {
//! [metafunction2]
template <template <typename ...> class F, typename ...T>
constexpr auto metafunction(_type<T> const& ...)
{ return type<typename F<T...>::type>; }

BOOST_HANA_CONSTANT_CHECK(
  metafunction<std::common_type>(type<int>, type<long>) == type<long>
);
//! [metafunction2]
}

namespace _template {
//! [template_]
template <template <typename ...> class F, typename ...T>
constexpr auto template_(_type<T> const& ...)
{ return type<F<T...>>; }

BOOST_HANA_CONSTANT_CHECK(
  template_<std::vector>(type<int>) == type<std::vector<int>>
);
//! [template_]
}
