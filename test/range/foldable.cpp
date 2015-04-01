/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/foldable.hpp>

#include <test/cnumeric.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto ranges = make<Tuple>(
          range(int_<0>, int_<0>)
        , range(int_<0>, int_<1>)
        , range(int_<0>, int_<2>)
        , range(int_<1>, int_<1>)
        , range(int_<1>, int_<2>)
        , range(int_<1>, int_<3>)
        , range(int_<50>, int_<60>)

        , range(int_<50>, long_<60>)
        , range(long_<50>, int_<60>)
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<0>), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<1>), f),
                f(int_<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<2>), f),
                f(int_<0>, int_<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(range(int_<0>, int_<3>), f),
                f(int_<0>, int_<1>, int_<2>)
            ));

            // Previously, we would only unpack with `std::size_t`s. Make
            // sure this does not happen.
            unpack(range(int_<0>, int_<1>), [](auto x) {
                using T = datatype_t<decltype(x)>;
                static_assert(std::is_same<typename T::value_type, int>{}, "");
            });
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<0>)), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<1>)), size_t<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<0>, int_<2>)), size_t<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<4>)), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(range(int_<4>, int_<10>)), size_t<6>
            ));
        }

        // minimum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<3>, int_<5>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                minimum(range(int_<-1>, int_<5>)), int_<-1>
            ));
        }

        // maximum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<3>, int_<5>)), int_<4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                maximum(range(int_<-1>, int_<6>)), int_<5>
            ));
        }

        // sum
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<-3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<-1>)), int_<-3 + -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<0>)), int_<-3 + -2 + -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<1>)), int_<-3 + -2 + -1 + 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<2>)), int_<-3 + -2 + -1 + 0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<-3>, int_<3>)), int_<-3 + -2 + -1 + 0 + 1 + 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<0>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<1>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<2>)), int_<0 + 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<3>)), int_<0 + 1 + 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<0>, int_<4>)), int_<0 + 1 + 2 + 3>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<3>)), int_<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<5>)), int_<3 + 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<6>)), int_<3 + 4 + 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                sum<>(range(int_<3>, int_<7>)), int_<3 + 4 + 5 + 6>
            ));
        }

        // product
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-2>)), int_<-3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<-1>)), int_<-3 * -2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<0>)), int_<-3 * -2 * -1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<1>)), int_<-3 * -2 * -1 * 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<2>)), int_<-3 * -2 * -1 * 0 * 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<-3>, int_<3>)), int_<-3 * -2 * -1 * 0 * 1 * 2>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<1>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<2>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<3>)), int_<1 * 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<4>)), int_<1 * 2 * 3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<1>, int_<5>)), int_<1 * 2 * 3 * 4>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<3>)), int_<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<4>)), int_<3>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<5>)), int_<3 * 4>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<6>)), int_<3 * 4 * 5>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                product<>(range(int_<3>, int_<7>)), int_<3 * 4 * 5 * 6>
            ));
        }

        // laws
        test::TestFoldable<Range>{ranges};
    }
}
