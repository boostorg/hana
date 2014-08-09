/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
using namespace boost::hana;


struct UDT {
    struct Type1 { };
    struct Type2 { };
    Type1 member1;
    Type2 member2;
};

template <typename Mcd, typename T, typename U>
void test() {
    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            detail::cnumber<T, 0, Mcd>,
            detail::cnumber<U, 0, Mcd>
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            detail::cnumber<T, 0, Mcd>,
            detail::cnumber<U, 1, Mcd>
        )));

        // Incomparable constants must appear as unequal.
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            detail::cnumber<T, 0, Mcd>,
            detail::cnumber<decltype(&UDT::member1), &UDT::member1, Mcd>
        )));
        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            detail::cnumber<decltype(&UDT::member1), &UDT::member1, Mcd>,
            detail::cnumber<decltype(&UDT::member2), &UDT::member2, Mcd>
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable::laws::check(
            list(
                detail::cnumber<T, 0, Mcd>,
                detail::cnumber<T, 1, Mcd>,

                detail::cnumber<U, 1, Mcd>,
                detail::cnumber<U, 2, Mcd>,

                detail::cnumber<decltype(&UDT::member1), &UDT::member1, Mcd>
            )
        ));
    }
}

int main() {
    test<Constant::mcd, int, int>();
    test<Constant::mcd, int, long>();
    test<Constant::mcd, int, unsigned long long>();
}
