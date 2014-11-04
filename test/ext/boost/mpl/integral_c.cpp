/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral.hpp>
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
#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::mpl::IntegralC> = tuple(
        type<Constant>,
        type<IntegralConstant>
    );

    template <>
    auto objects<ext::boost::mpl::IntegralC> = tuple(
        ::boost::mpl::integral_c<int, 0>{},
        ::boost::mpl::integral_c<int, 1>{},
        ::boost::mpl::integral_c<int, 2>{},
        ::boost::mpl::integral_c<int, 3>{},

        ::boost::mpl::true_{},
        ::boost::mpl::false_{},

        ::boost::mpl::long_<10>{},
        ::boost::mpl::char_<'c'>{}
    );
}}}


int main() {
    test::check_datatype<ext::boost::mpl::IntegralC>();

    // make sure the datatype is correct
    {
        static_assert(std::is_same<
            datatype_t< ::boost::mpl::bool_<true>>,
            ext::boost::mpl::IntegralC
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::int_<0>>,
            ext::boost::mpl::IntegralC
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::long_<0>>,
            ext::boost::mpl::IntegralC
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::size_t<0>>,
            ext::boost::mpl::IntegralC
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::integral_c<int, 0>>,
            ext::boost::mpl::IntegralC
        >::value, "");

        static_assert(std::is_same<
            datatype_t< ::boost::mpl::char_<0>>,
            ext::boost::mpl::IntegralC
        >::value, "");
    }

    // interoperation with Integral
    {
        BOOST_HANA_CONSTANT_ASSERT(::boost::mpl::integral_c<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_ASSERT(::boost::mpl::integral_c<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_ASSERT(::boost::mpl::integral_c<int, 2>{} != int_<3>);
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
