/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/core/datatype.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename ...xs>
struct check_datatype {
    static_assert(std::is_same<
        datatype_t<boost::mpl::vector<xs...>>,
        BoostMplVector
    >{}, "");

    static_assert(std::is_same<
        datatype_t<typename boost::mpl::vector<xs...>::type>,
        BoostMplVector
    >{}, "");

    struct mpl_id { template <typename x> struct apply { using type = x; }; };
    static_assert(std::is_same<
        datatype_t<typename boost::mpl::transform<boost::mpl::vector<xs...>, mpl_id>::type>,
        BoostMplVector
    >{}, "");
};

template struct check_datatype<>;
template struct check_datatype<int>;
template struct check_datatype<int, char>;
template struct check_datatype<int, char, void>;
template struct check_datatype<int, char, void, struct foo>;

int main() { }
