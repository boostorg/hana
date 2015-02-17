/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/sequence.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

#include <boost/tuple/tuple.hpp>
#include <type_traits>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::Tuple> = tuple(
        type<Sequence>
    );

    template <>
    auto objects<ext::boost::Tuple> = tuple(
        ::boost::make_tuple(),
        ::boost::make_tuple(ord<0>),
        ::boost::make_tuple(ord<0>, ord<1>),
        ::boost::make_tuple(ord<0>, ord<1>, ord<2>),
        ::boost::make_tuple(ord<0>, ord<1>, ord<2>, ord<3>)
    );
}}}


int main() {
    test::check_datatype<ext::boost::Tuple>();

    // make sure the datatype is correct
    {
        auto make_cons = [](auto x, auto xs) {
            return boost::tuples::cons<decltype(x), decltype(xs)>{x, xs};
        };

        static_assert(std::is_same<
            datatype_t<decltype(boost::make_tuple())>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(boost::make_tuple(1))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(boost::make_tuple(1, '2'))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(boost::make_tuple(1, '2', 3.3))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(make_cons(1, boost::tuples::null_type{}))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(make_cons(1, make_cons('2', boost::tuples::null_type{})))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<decltype(make_cons(1, boost::make_tuple('2', 3.3)))>,
            ext::boost::Tuple
        >::value, "");

        static_assert(std::is_same<
            datatype_t<boost::tuples::null_type>,
            ext::boost::Tuple
        >::value, "");
    }
}
