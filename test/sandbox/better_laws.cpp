/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>

#include <cassert>
using namespace boost::hana;


auto implies = [](auto p, auto q) { return or_(not_(p), q); };

auto Comparable_laws = [](auto comparable_objects) {
    for_each(comparable_objects, [=](auto a) {
        for_each(comparable_objects, [=](auto b) {
            for_each(comparable_objects, [=](auto c) {
                assert(equal(a, a));
                assert(implies(equal(a, b), equal(b, a)));
                assert(implies(and_(equal(a, b), equal(b, c)), equal(a, c)));
            });
        });
    });
};

auto Functor_laws = [](auto functions_from_Y_to_Z, auto functions_from_X_to_Y, auto functors_on_X) {
    for_each(functors_on_X, [=](auto x) {
        assert(equal(fmap(id, x), x));
    });

    for_each(functions_from_Y_to_Z, [=](auto f) {
        for_each(functions_from_X_to_Y, [=](auto g) {
            for_each(functors_on_X, [=](auto x) {
                assert(equal(fmap(compose(f, g), x), fmap(f, fmap(g, x))));
            });
        });
    });
};

//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/list.hpp>


int main() {
    for_each(list(1, 2, 3), [](auto x) { });

    Comparable_laws(list(1, 2, 3, 4, 5));

    Functor_laws(
        list(
            [](auto x) { return x / 2; }
        ),
        list(
            [](auto x) { return x + 1.2; },
            [](auto x) { return static_cast<double>(x * x); },
            [](auto x) { return static_cast<double>(x); }
        ),
        list(
            list(),
            list(1),
            list(1, 2),
            list(1, 2, 3)
        )
    );
}
