/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/constant/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename Mcd, typename T>
void test() {
    // value
    {
        static_assert(value(detail::cnumber<T, 0, Mcd>) == 0, "");
        static_assert(value(detail::cnumber<T, 1, Mcd>) == 1, "");
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Constant::laws::check(
            list(
                detail::cnumber<T, 0, Mcd>,
                detail::cnumber<T, 1, Mcd>,
                detail::cnumber<T, 2, Mcd>,
                detail::cnumber<T, 3, Mcd>
            )
        ));
    }
}

int main() {
    test<Constant::mcd, int>();
    test<Constant::mcd, long>();
}
