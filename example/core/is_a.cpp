/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    //! [standard]
    BOOST_HANA_CONSTANT_CHECK( is_a<Foldable, Maybe>);
    BOOST_HANA_CONSTANT_CHECK(!is_a<Logical, Maybe>);
    BOOST_HANA_CONSTANT_CHECK(!is_a<Foldable, int>);
    //! [standard]

    //! [alternate]
    constexpr auto row = make<Tuple>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto check_table = [](auto ...headers) {
        return [=](auto ...rows) {
            auto row_is_correct = [=](auto row) {
                return make<Tuple>(headers(head(row))...) == tail(row);
            };
            BOOST_HANA_CONSTANT_CHECK(all(make<Tuple>(rows...), row_is_correct));
        };
    };

    check_table(                      is_a<Logical>, is_an<Iterable>, is_a<Monad>   )(
    row(just(1),                         false_,         false_,         true_      ),
    row(make<Tuple>(1, '2', 3.3),        false_,         true_,          true_      ),
    row(std::make_tuple("abc", 'd'),     false_,         true_,          true_      ),
    row(long_<12>,                       true_,          false_,         false_     ),
    row(range(int_<-4>, int_<15>),       false_,         true_,          false_     )
    );
    //! [alternate]
}
