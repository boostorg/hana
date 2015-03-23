/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/iterate.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/enumerable.hpp>

#include <laws/base.hpp>

#include <vector>
using namespace boost::hana;


struct undefined { };

int main() {
    test::_injection<0> f{};
    using test::ct_eq;

    // "real usage" tests
    static_assert(iterate<3>(succ, 0) == 3, "");
    {
        std::vector<int> vec;
        iterate<10>([&](int i) { vec.push_back(i); return i + 1; }, 0);
        BOOST_HANA_RUNTIME_CHECK(
            vec == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
        );
    }

    // equivalence between iterate<n>(f, x) and iterate<n>(f)(x)
    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<0>(undefined{})(ct_eq<0>{}),
        iterate<0>(undefined{}, ct_eq<0>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<4>(f)(ct_eq<0>{}),
        iterate<4>(f, ct_eq<0>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<10>(f)(ct_eq<0>{}),
        iterate<10>(f, ct_eq<0>{})
    ));

    // systematic tests
    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<0>(undefined{}, ct_eq<0>{}),
        ct_eq<0>{}
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<1>(f, ct_eq<0>{}),
        f(ct_eq<0>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<2>(f, ct_eq<0>{}),
        f(f(ct_eq<0>{}))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<3>(f, ct_eq<0>{}),
        f(f(f(ct_eq<0>{})))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<4>(f, ct_eq<0>{}),
        f(f(f(f(ct_eq<0>{}))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<5>(f, ct_eq<0>{}),
        f(f(f(f(f(ct_eq<0>{})))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<6>(f, ct_eq<0>{}),
        f(f(f(f(f(f(ct_eq<0>{}))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<7>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(ct_eq<0>{})))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<11>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(ct_eq<0>{})))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<12>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(ct_eq<0>{}))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<13>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(f(ct_eq<0>{})))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<23>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(
            ct_eq<0>{}
        ))))))))))))
        )))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<24>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
            ct_eq<0>{}
        ))))))))))))
        ))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<25>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(
            ct_eq<0>{}
        ))))))))))))
        ))))))))))))
        )
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<47>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(
            ct_eq<0>{}
        ))))))))))))
        ))))))))))))
        ))))))))))))
        )))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<48>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
            ct_eq<0>{}
        ))))))))))))
        ))))))))))))
        ))))))))))))
        ))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        iterate<49>(f, ct_eq<0>{}),
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(f(f(f(f(f(f(f(f(f(f(f(
        f(
            ct_eq<0>{}
        ))))))))))))
        ))))))))))))
        ))))))))))))
        ))))))))))))
        )
    ));
}
