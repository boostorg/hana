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

// instances
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::mpl::Vector> = tuple(
        //! @todo mpl::Vector is not actually a List, because we can only
        //! store types in it.
#if 0
        type<List>,
#endif
        type<Iterable>
    );

    template <>
    auto objects<ext::boost::mpl::Vector> = tuple(
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

    // List
    {
        // cons
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                cons(type<x1>, ::boost::mpl::vector<>{}),
                ::boost::mpl::vector<x1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cons(type<x1>, ::boost::mpl::vector<x2>{}),
                ::boost::mpl::vector<x1, x2>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                cons(type<x1>, ::boost::mpl::vector<x2, x3>{}),
                ::boost::mpl::vector<x1, x2, x3>{}
            ));
        }

        // nil
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                nil<ext::boost::mpl::Vector>,
                ::boost::mpl::vector<>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                nil<ext::boost::mpl::Vector>,
                ::boost::mpl::vector0<>{}
            ));
        }
    }
}
