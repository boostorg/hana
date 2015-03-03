/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <boost/mpl/if.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
using namespace boost::hana;


int main() {

{

//! [type]
// We "lift" the `int` type to a value, and the `std::add_pointer` metafunction
// to a regular function. Then, we can call that metafunction with a regular
// function call syntax.
auto t = type<int>;
auto add_pointer = metafunction<std::add_pointer>;
static_assert(add_pointer(t) == type<int*>, "");
//! [type]

//! [type_as_nullary_metafunction]
using T = decltype(t)::type;
static_assert(std::is_same<T, int>{}, "");
//! [type_as_nullary_metafunction]

}{

//! [type_sequence]
auto types = make<Tuple>(type<int>, type<char const>, type<void>);
static_assert(
    transform(types, metafunction<std::add_pointer>) ==
    make<Tuple>(type<int*>, type<char const*>, type<void*>)
, "");
//! [type_sequence]

}{

//! [tuple_t]
auto types = tuple_t<int, char const, void>;
static_assert(types == make<Tuple>(type<int>, type<char const>, type<void>), "");

static_assert(
    transform(types, metafunction<std::add_pointer>) ==
    tuple_t<int*, char const*, void*>
, "");
//! [tuple_t]

}{

//! [type_three_step_cumbersome]
static_assert(std::is_same<
    decltype(metafunction<std::add_pointer>(type<int>))::type,
    int*
>{}, "");
//! [type_three_step_cumbersome]

//! [type_three_step_alternative]
static_assert(std::is_same<
    std::add_pointer<int>::type,
    int*
>{}, "");
//! [type_three_step_alternative]

}{

//! [apply_to_all]
auto apply_to_all = [](auto sequences, auto f) {
    return transform(sequences, [=](auto sequence) {
        return transform(sequence, f);
    });
};

auto types = make<Tuple>(
    tuple_t<int, char>,
    tuple_t<void, std::string, double>
);

BOOST_HANA_CONSTANT_CHECK(
    apply_to_all(types, metafunction<std::add_pointer>) ==
    make<Tuple>(
        tuple_t<int*, char*>,
        tuple_t<void*, std::string*, double*>
    )
);
//! [apply_to_all]

}

}

namespace mpl = boost::mpl;
//! [apply_to_all_mpl]
template <typename Sequences, typename F>
struct apply_to_all
    : mpl::transform<
        Sequences,
        mpl::transform<mpl::_1, F>
    >
{ };

using types = mpl::vector<
    mpl::vector<int, char>,
    mpl::vector<void, std::string, double>
>;

static_assert(mpl::equal<
    apply_to_all<types, mpl::quote1<std::add_pointer>>::type,
    mpl::vector<
        mpl::vector<int*, char*>,
        mpl::vector<void*, std::string*, double*>
    >,

    // mpl::equal was seemingly not designed for deep comparisons, so we
    // need this tricky line for it to do what we want.
    mpl::equal<mpl::_1, mpl::_2, mpl::quote2<std::is_same>>
>{}, "");
//! [apply_to_all_mpl]
