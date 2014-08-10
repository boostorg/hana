/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


template <typename> struct F;
constexpr auto f = template_<F>;

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto t) {
    return type<F<typename decltype(t)::type>>;
};

template <int ...>
struct result {
    template <typename T>
    struct apply { using type = apply; };
};

struct x0; struct x1; struct x2;


int main() {
    (void)g;

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<>{}), mpl::vector<>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0>{}), mpl::vector<F<x0>>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0, x1>{}), mpl::vector<F<x0>, F<x1>>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, mpl::vector<x0, x1, x2>{}), mpl::vector<F<x0>, F<x1>, F<x2>>{}));

        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<>{}), mpl::vector<>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0>{}), mpl::vector<F<x0>>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0, x1>{}), mpl::vector<F<x0>, F<x1>>{}));
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(g, mpl::vector<x0, x1, x2>{}), mpl::vector<F<x0>, F<x1>, F<x2>>{}));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Functor::laws::check(
            list(
                mpl::vector<>{},
                mpl::vector<x0>{},
                mpl::vector<x0, x1>{},
                mpl::vector<x0, x1, x2>{}
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
}
