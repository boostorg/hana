/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/list.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functor/laws.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/list.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <int ...>
struct result {
    template <typename T>
    struct apply { using type = apply; };
};

struct x0; struct x1; struct x2;

int main() {
    BOOST_HANA_CONSTANT_ASSERT(Functor::laws::check(
        list(
            mpl::list<>{},
            mpl::list<x0>{},
            mpl::list<x0, x1>{},
            mpl::list<x0, x1, x2>{}
        ),
        list(
            metafunction_class<result<1, 1>>,
            metafunction_class<result<1, 2>>
        ),
        list(
            metafunction_class<result<2, 1>>,
            metafunction_class<result<2, 2>>
        )
    ));
}
