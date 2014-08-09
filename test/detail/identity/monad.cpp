/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/identity/monad.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monad/detail/laws.hpp>
using namespace boost::hana;


template <typename Mcd>
void test() {
    constexpr auto monad = detail::identity<Mcd>;
    using M = detail::Identity<Mcd>;
    constexpr auto comparable = detail::number<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(monad, detail::injection([]{}));
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(monad, detail::injection([]{}));

    // bind
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            bind(monad(comparable(1)), f),
            f(comparable(1))
        ));
    }

    // tap
    {
        bool executed = false;
        BOOST_HANA_CONSTEXPR_LAMBDA auto exec = [&](auto) { executed = true; };
        BOOST_HANA_RUNTIME_ASSERT(equal(
            bind(monad(comparable(0)), tap<M>(exec)),
            monad(comparable(0))
        ));
        BOOST_HANA_RUNTIME_ASSERT(executed);
    }

    // then
    {
        struct invalid { };
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            then(monad(invalid{}), monad(comparable(0))),
            monad(comparable(0))
        ));
    }

    // operators
    {
        auto monad = detail::identity<Mcd, operators<Monad>>;
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(monad, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(monad, detail::injection([]{}));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            monad(comparable(1)) | f,
            bind(monad(comparable(1)), f)
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            monad(comparable(1)) | f | g,
            bind(bind(monad(comparable(1)), f), g)
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check2(
            list(
                monad(comparable(0)),
                monad(comparable(1)),
                monad(comparable(2))
            ),
            list(
                comparable(0),
                comparable(1),
                comparable(2)
            ),
            list(f),
            list(g)
        ));
    }
}

int main() {
    test<detail::quote<Monad::flatten_mcd>>();
    test<detail::quote<Monad::bind_mcd>>();
}
