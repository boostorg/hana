/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

// instances
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>

#include <boost/tuple/tuple.hpp>
#include <test/injection.hpp>
#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::Tuple> = tuple(
        type<List>,
        type<Iterable>
    );

    template <>
    auto objects<ext::boost::Tuple> = tuple(
        ::boost::make_tuple(),
        ::boost::make_tuple(x<0>),
        ::boost::make_tuple(x<0>, x<1>),
        ::boost::make_tuple(x<0>, x<1>, x<2>),
        ::boost::make_tuple(x<0>, x<1>, x<2>, x<3>)
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
