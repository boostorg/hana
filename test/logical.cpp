/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/logical.hpp>

#include <vector>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <> auto objects<bool> = make<Tuple>(true, false);
}}}


int main() {
    test::laws<Logical, bool>();

    // eval_if
    {
        BOOST_HANA_CONSTEXPR_CHECK(equal(
            eval_if(true, always(1), always(2)),
            1
        ));

        BOOST_HANA_CONSTEXPR_CHECK(equal(
            eval_if(false, always(1), always(2)),
            2
        ));
    }

    // not_
    {
        BOOST_HANA_CONSTEXPR_CHECK(equal(not_(true), false));
        BOOST_HANA_CONSTEXPR_CHECK(equal(not_(false), true));
    }

    // until
    {
        auto has_length = [](auto n) {
            return [n](auto v) { return v.size() == n; };
        };
        auto f = [](auto v) {
            v.push_back(v.size());
            return v;
        };

        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(0u), std::vector<int>{}, f),
            std::vector<int>{}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(1u), std::vector<int>{}, f),
            std::vector<int>{0}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(2u), std::vector<int>{}, f),
            std::vector<int>{0, 1}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(3u), std::vector<int>{}, f),
            std::vector<int>{0, 1, 2}
        ));

        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(4u), std::vector<int>{}, f),
            std::vector<int>{0, 1, 2, 3}
        ));

        // Make sure it can be called with an lvalue state:
        std::vector<int> v{};
        BOOST_HANA_RUNTIME_CHECK(equal(
            until(has_length(4u), v, f),
            std::vector<int>{0, 1, 2, 3}
        ));
    }
}
