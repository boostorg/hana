/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>

#include <boost/mpl/vector.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    //! [main]
    using namespace operators; // required to be able to == MPL vectors
    BOOST_HANA_STATIC_ASSERT(
        mpl::vector2<int, char>{} == mpl::vector<int, char>{}
    );

    BOOST_HANA_STATIC_ASSERT(
        mpl::vector2<int, char>{} != mpl::vector<int, char, float>{}
    );
    //! [main]
}
