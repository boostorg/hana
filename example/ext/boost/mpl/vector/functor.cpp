/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
#include <type_traits>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    //! [main]
    using namespace operators; // required to be able to == MPL vectors
    constexpr mpl::vector<int, char, float> types{};

    BOOST_HANA_CONSTANT_ASSERT(
        fmap(metafunction<std::add_pointer>, types)
        ==
        mpl::vector<int*, char*, float*>{}
    );

    BOOST_HANA_CONSTANT_ASSERT(
        replace(trait<std::is_floating_point>, type<void>, types)
        ==
        mpl::vector<int, char, void>{}
    );
    //! [main]
}
