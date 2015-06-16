/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <laws/base.hpp>

#include <utility>
#include <memory>
using namespace boost::hana;


int main() {
    {
        using T = detail::closure<std::unique_ptr<int>>;
        T t(std::unique_ptr<int>(new int(3)));
        std::unique_ptr<int> p = detail::get<0>(std::move(t));
        BOOST_HANA_RUNTIME_CHECK(*p == 3);
    }
    // make sure we don't double-move and do other weird stuff
    {
        detail::closure<test::Tracked, test::Tracked, test::Tracked> xs{
            test::Tracked{1}, test::Tracked{2}, test::Tracked{3}
        };

        test::Tracked a = detail::get<0>(std::move(xs)); (void)a;
        test::Tracked b = detail::get<1>(std::move(xs)); (void)b;
        test::Tracked c = detail::get<2>(std::move(xs)); (void)c;
    }
    // test with nested closures
    {
        using Inner = detail::closure<test::Tracked, test::Tracked>;
        detail::closure<Inner> xs{Inner{test::Tracked{1}, test::Tracked{2}}};

        test::Tracked a = detail::get<0>(detail::get<0>(std::move(xs))); (void)a;
        test::Tracked b = detail::get<1>(detail::get<0>(std::move(xs))); (void)b;
    }
}
