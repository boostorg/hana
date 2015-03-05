/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/char.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/size_t.hpp>
#include <cstddef>
#include <type_traits>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // make sure the datatype is correct
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(std::is_same<
            datatype_t<mpl::bool_<true>>,
            ext::boost::mpl::IntegralC<bool>
        >::value, "");

        static_assert(std::is_same<
            datatype_t<mpl::int_<0>>,
            ext::boost::mpl::IntegralC<int>
        >::value, "");

        static_assert(std::is_same<
            datatype_t<mpl::long_<0>>,
            ext::boost::mpl::IntegralC<long>
        >::value, "");

        static_assert(std::is_same<
            datatype_t<mpl::size_t<0>>,
            ext::boost::mpl::IntegralC<std::size_t>
        >::value, "");

        static_assert(std::is_same<
            datatype_t<mpl::integral_c<int, 0>>,
            ext::boost::mpl::IntegralC<int>
        >::value, "");

        static_assert(std::is_same<
            datatype_t<mpl::char_<0>>,
            ext::boost::mpl::IntegralC<char>
        >::value, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // interoperation with IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(mpl::integral_c<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_CHECK(mpl::integral_c<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_CHECK(mpl::integral_c<int, 2>{} != int_<3>);
    }
}
