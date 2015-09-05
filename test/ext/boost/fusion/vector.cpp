/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/vector.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/tail.hpp>

#include <laws/base.hpp>

#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/vector.hpp>
namespace hana = boost::hana;
namespace fusion = boost::fusion;


using hana::test::ct_eq;

struct undefined { };

int main() {
    //////////////////////////////////////////////////////////////////////////
    // make<Vector>
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            fusion::make_vector(),
            hana::make<hana::ext::boost::fusion::vector_tag>()
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            fusion::make_vector(ct_eq<0>{}),
            hana::make<hana::ext::boost::fusion::vector_tag>(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}),
            hana::make<hana::ext::boost::fusion::vector_tag>(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
            hana::make<hana::ext::boost::fusion::vector_tag>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // tail
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::tail(fusion::make_vector(ct_eq<0>{})),
            fusion::make_vector()
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::tail(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{})),
            fusion::make_vector(ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::tail(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})),
            fusion::make_vector(ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::tail(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})),
            fusion::make_vector(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // at
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<0>(fusion::make_vector(ct_eq<0>{})),
            ct_eq<0>{}
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<0>(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{})),
            ct_eq<0>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<1>(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{})),
            ct_eq<1>{}
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<0>(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})),
            ct_eq<0>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<1>(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})),
            ct_eq<1>{}
        ));
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::at_c<2>(fusion::make_vector(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})),
            ct_eq<2>{}
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // is_empty
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(hana::is_empty(
            fusion::make_vector()
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(
            fusion::make_vector(undefined{})
        )));

        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(
            fusion::make_vector(undefined{}, undefined{})
        )));
    }

    //////////////////////////////////////////////////////////////////////////
    // length
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(fusion::make_vector()),
            hana::size_c<0>
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(fusion::make_vector(undefined{})),
            hana::size_c<1>
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(fusion::make_vector(undefined{}, undefined{})),
            hana::size_c<2>
        ));

        BOOST_HANA_CONSTANT_CHECK(hana::equal(
            hana::length(fusion::make_vector(undefined{}, undefined{}, undefined{})),
            hana::size_c<3>
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(hana::Sequence<hana::ext::boost::fusion::vector_tag>::value, "");
    }
}
