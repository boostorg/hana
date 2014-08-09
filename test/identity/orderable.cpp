/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/identity.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/number/orderable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/orderable/detail/laws.hpp>
using namespace boost::hana;


auto implies = infix([](auto p, auto q) {
    return or_(not_(p), q);
});

auto iff = infix([](auto p, auto q) {
    return and_(p ^implies^ q, q ^implies^ p);
});

auto check = [](auto x, auto y) {
    return less(x, y) ^iff^ less(identity(x), identity(y));
};

int main() {
    constexpr auto x = detail::number<>(1);
    constexpr auto y = detail::number<>(2);
    constexpr auto z = detail::number<>(3);

    BOOST_HANA_CONSTEXPR_ASSERT(
        all_of(ap(list(check), list(x, y, z), list(x, y, z)))
    );

    BOOST_HANA_CONSTEXPR_ASSERT(Orderable::laws::check(
        list(identity(x), identity(y), identity(z))
    ));
}
