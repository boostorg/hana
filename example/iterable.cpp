/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <type_traits>
using namespace boost::hana;


int main() {
    {
        //! [head]
        BOOST_HANA_CONSTEXPR_ASSERT(head(tuple(1, '2', 3.3, nullptr)) == 1);
        //! [head]
    }

    {
        //! [at]
        BOOST_HANA_CONSTEXPR_ASSERT(at(int_<0>, tuple(0, '1', 2.0)) == 0);
        BOOST_HANA_CONSTEXPR_ASSERT(at(int_<1>, tuple(0, '1', 2.0)) == '1');
        BOOST_HANA_CONSTEXPR_ASSERT(at(int_<2>, tuple(0, '1', 2.0)) == 2.0);
        //! [at]
    }

    {
        //! [at_c]
        BOOST_HANA_CONSTEXPR_ASSERT(at_c<0>(tuple(0, '1', 2.0)) == 0);
        BOOST_HANA_CONSTEXPR_ASSERT(at_c<1>(tuple(0, '1', 2.0)) == '1');
        BOOST_HANA_CONSTEXPR_ASSERT(at_c<2>(tuple(0, '1', 2.0)) == 2.0);
        //! [at_c]
    }

    {
        //! [drop]
        BOOST_HANA_CONSTEXPR_LAMBDA auto xs = tuple(0, '1', 2.0);
        BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<0>, xs) == xs);
        BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<1>, xs) == tuple('1', 2.0));
        BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<2>, xs) == tuple(2.0));
        BOOST_HANA_CONSTANT_ASSERT(drop(int_<3>, xs) == tuple());
        BOOST_HANA_CONSTANT_ASSERT(drop(int_<4>, xs) == tuple());
        //! [drop]
    }

    {
        //! [drop_c]
        BOOST_HANA_CONSTEXPR_ASSERT(drop_c<2>(tuple(0, '1', 2.0)) == tuple(2.0));
        //! [drop_c]
    }

    {
        //! [drop_until]
        using namespace literals;
        BOOST_HANA_CONSTEXPR_LAMBDA auto positive = [](auto x) {
            return x > int_<0>;
        };

        BOOST_HANA_CONSTANT_ASSERT(
            drop_until(range(int_<-3>, int_<6>), positive) == range(int_<1>, int_<6>)
        );

        BOOST_HANA_CONSTANT_ASSERT(
            drop_until(tuple(1_c, -2_c, 4_c, 5_c), positive) == tuple(1_c, -2_c, 4_c, 5_c)
        );
        //! [drop_until]
    }

    {
        //! [drop_while]
        using namespace literals;
        BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
            return x < int_<0>;
        };

        BOOST_HANA_CONSTANT_ASSERT(
            drop_while(range(int_<-3>, int_<6>), negative) == range(int_<0>, int_<6>)
        );

        BOOST_HANA_CONSTANT_ASSERT(
            drop_while(tuple(1_c, -2_c, 4_c, 5_c), negative) == tuple(1_c, -2_c, 4_c, 5_c)
        );
        //! [drop_while]
    }

    {
        //! [is_empty]
        BOOST_HANA_CONSTANT_ASSERT(!is_empty(tuple(1, '2')));
        BOOST_HANA_CONSTANT_ASSERT( is_empty(tuple()));
        //! [is_empty]
    }

    {
        //! [last]
        BOOST_HANA_CONSTEXPR_ASSERT(last(tuple(1, '2', 3.3, nullptr)) == nullptr);
        //! [last]
    }

    {
        //! [tail]
        BOOST_HANA_CONSTEXPR_ASSERT(
            tail(tuple(1, '2', 3.3, nullptr)) == tuple('2', 3.3, nullptr)
        );
        //! [tail]
    }

    // iterable_mcd
    {
        //! [find]
        BOOST_HANA_CONSTEXPR_ASSERT(find(tuple(1.0, 2, '3'), trait_<std::is_integral>) == just(2));
        BOOST_HANA_CONSTANT_ASSERT(find(tuple(1.0, 2, '3'), trait_<std::is_class>) == nothing);
        //! [find]
    }
}
