/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/cnumeric.hpp>
#include <test/identity.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/applicative.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/monad.hpp>
#include <test/auto/orderable.hpp>
#include <test/auto/traversable.hpp>

#include <type_traits>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Either> = make<Tuple>(
          type<Comparable>
        , type<Orderable>

        , type<Functor>
        , type<Applicative>
        , type<Monad>

        , type<Foldable>
        , type<Traversable>
    );

    template <>
    auto objects<Either> = make<Tuple>(
        right(ord<0>), left(ord<0>),
        right(ord<1>), left(ord<1>),
        right(ord<2>), left(ord<2>),
        right(ord<3>), left(ord<3>)
    );
}}}


int main() {
    test::check_datatype<Either>();

    auto f = test::injection([]{});
    auto g = test::injection([]{});
    auto x = test::injection([]{})();
    auto y = test::injection([]{})();
    constexpr struct { } undefined{};

    // Interface
    {
        // left
        {
            auto e = left(undefined);
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // right
        {
            auto e = right(undefined);
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // either
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, left(x)),
                f(x)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, right(x)),
                g(x)
            ));
        }
    }

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(left(x), left(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(left(x), left(y))));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                left(undefined), right(undefined)
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                right(undefined), left(undefined)
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(right(x), right(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(right(x), right(y))));
        }
    }

    // Orderable
    {
        // less
        {
            BOOST_HANA_CONSTANT_CHECK(less(left(ord<0>), left(ord<1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(less(left(ord<0>), left(ord<0>))));
            BOOST_HANA_CONSTANT_CHECK(not_(less(left(ord<1>), left(ord<0>))));

            BOOST_HANA_CONSTANT_CHECK(less(right(ord<0>), right(ord<1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(less(right(ord<0>), right(ord<0>))));
            BOOST_HANA_CONSTANT_CHECK(not_(less(right(ord<1>), right(ord<0>))));

            BOOST_HANA_CONSTANT_CHECK(less(left(ord<0>), right(ord<1>)));
            BOOST_HANA_CONSTANT_CHECK(less(left(ord<0>), right(ord<0>)));
            BOOST_HANA_CONSTANT_CHECK(less(left(ord<1>), right(ord<0>)));
        }
    }

    // Functor
    {
        // transform
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(left(x), undefined), left(x)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(right(x), f), right(f(x))
            ));
        }
    }

    // Applicative
    {
        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(x), left(undefined)),
                left(x)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(x), right(undefined)),
                left(x)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(undefined), left(x)),
                left(x)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(f), right(x)),
                right(f(x))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(lift<Either>(x), right(x)));
        }
    }

    // Monad
    {
        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(left(x))),
                left(left(x))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(right(x))),
                left(right(x))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(left(x))),
                left(x)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(right(x))),
                right(x)
            ));
        }
    }


    // Foldable
    {
        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(left(x), f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(right(x), f),
                f(x)
            ));
        }
    }

    // Traversable
    {
        auto f_ = compose(lift<test::Identity>, f);
        BOOST_HANA_CONSTANT_CHECK(equal(
            traverse<test::Identity>(left(x), f_), test::identity(left(x))
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            traverse<test::Identity>(right(x), f_), test::identity(right(f(x)))
        ));
    }
}
