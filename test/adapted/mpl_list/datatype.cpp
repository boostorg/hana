/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/mpl_list.hpp>

#include <boost/hana/core.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/transform.hpp>
#include <type_traits>
using namespace boost::hana;


template <typename ...xs>
struct check_datatype {
    static_assert(std::is_same<
        datatype_t<boost::mpl::list<xs...>>,
        MplList
    >{}, "");

    static_assert(std::is_same<
        datatype_t<typename boost::mpl::list<xs...>::type>,
        MplList
    >{}, "");

    struct mpl_id { template <typename x> struct apply { using type = x; }; };
    static_assert(std::is_same<
        datatype_t<typename boost::mpl::transform<boost::mpl::list<xs...>, mpl_id>::type>,
        MplList
    >{}, "");
};

template struct check_datatype<>;
template struct check_datatype<int>;
template struct check_datatype<int, char>;
template struct check_datatype<int, char, void>;
template struct check_datatype<int, char, void, struct foo>;

int main() { }
