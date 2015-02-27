/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/searchable.hpp>
#include <test/injection.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::mpl::Vector> = make<Tuple>(
        type<Comparable>,
        type<Foldable>,
        type<Iterable>,
        type<Searchable>
    );

    template <>
    auto objects<ext::boost::mpl::Vector> = make<Tuple>(
        ::boost::mpl::vector<>{},
        ::boost::mpl::vector<x1>{},
        ::boost::mpl::vector<x1, x2>{},
        ::boost::mpl::vector<x1, x2, x3>{}
    );
}}}


int main() {
    test::check_datatype<ext::boost::mpl::Vector>();

    // make sure the data type is resolved correctly
    {
        using mpl_id = boost::mpl::quote1<boost::mpl::identity>;

        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<>>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<>::type>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::transform<boost::mpl::vector<>, mpl_id>::type>,
            ext::boost::mpl::Vector
        >{}, "");

        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<x1>>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<x1>::type>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::transform<boost::mpl::vector<x1>, mpl_id>::type>,
            ext::boost::mpl::Vector
        >{}, "");

        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<x1, x2>>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::vector<x1, x2>::type>,
            ext::boost::mpl::Vector
        >{}, "");
        static_assert(std::is_same<
            datatype_t<boost::mpl::transform<boost::mpl::vector<x1, x2>, mpl_id>::type>,
            ext::boost::mpl::Vector
        >{}, "");
    }

    // Foldable
    {
        // unpack
        {
            auto f = test::injection([]{});
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(::boost::mpl::vector<>{}, f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(::boost::mpl::vector<x1>{}, f),
                f(type<x1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(::boost::mpl::vector<x1, x2>{}, f),
                f(type<x1>, type<x2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(::boost::mpl::vector<x1, x2, x3>{}, f),
                f(type<x1>, type<x2>, type<x3>)
            ));
        }
    }

    // Iterable
    {
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(::boost::mpl::vector<x1>{}),
                type<x1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(::boost::mpl::vector<x1, x2>{}),
                type<x1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(::boost::mpl::vector<x1, x2, x3>{}),
                type<x1>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(::boost::mpl::vector<x1>{}),
                ::boost::mpl::vector<>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(::boost::mpl::vector<x1, x2>{}),
                ::boost::mpl::vector<x2>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(::boost::mpl::vector<x1, x2, x3>{}),
                ::boost::mpl::vector<x2, x3>{}
            ));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(::boost::mpl::vector<>{}));
            BOOST_HANA_CONSTANT_CHECK(is_empty(::boost::mpl::vector0<>{}));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(::boost::mpl::vector<x1>{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(::boost::mpl::vector1<x1>{})));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(::boost::mpl::vector<x1, x2>{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(::boost::mpl::vector2<x1, x2>{})));
        }
    }
}
