/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>

// instances
#include <test/auto/constant.hpp>
#include <test/auto/integral_constant.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/size_t.hpp>
#include <cstddef>
#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto instances<ext::boost::mpl::IntegralC<T>> = tuple(
        type<Constant>,
        type<IntegralConstant>
    );

    template <typename T>
    auto objects<ext::boost::mpl::IntegralC<T>> = tuple(
        ::boost::mpl::integral_c<T, 0>{},
        ::boost::mpl::integral_c<T, 1>{},
        ::boost::mpl::integral_c<T, 2>{},
        ::boost::mpl::integral_c<T, 3>{}
    );
}}}


int main() {
    test::check_datatype<ext::boost::mpl::IntegralC<int>>();

    // make sure the datatype is correct
    {
        static_assert(std::is_same<
            datatype_t< ::boost::mpl::bool_<true>>,
            ext::boost::mpl::IntegralC<bool>
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::int_<0>>,
            ext::boost::mpl::IntegralC<int>
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::long_<0>>,
            ext::boost::mpl::IntegralC<long>
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::size_t<0>>,
            ext::boost::mpl::IntegralC<std::size_t>
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::integral_c<int, 0>>,
            ext::boost::mpl::IntegralC<int>
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::char_<0>>,
            ext::boost::mpl::IntegralC<char>
        >::value, "");
    }

    // interoperation with Integral
    {
        BOOST_HANA_CONSTANT_CHECK(::boost::mpl::integral_c<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_CHECK(::boost::mpl::integral_c<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_CHECK(::boost::mpl::integral_c<int, 2>{} != int_<3>);
    }

    // Constant
    {
        // value
        {
            static_assert(value(::boost::mpl::integral_c<int, 0>{}) == 0, "");
            static_assert(value(::boost::mpl::integral_c<int, 1>{}) == 1, "");
        }
    }
}
