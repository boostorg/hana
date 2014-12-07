/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <array>
#include <iostream>
#include <test/auto/base.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    // We provide this instance for unit tests only because it is _so_ much
    // more convenient, but this instance is too dangerous for general usage.
    // See the documentation of `Lazy` for more info.
    template <typename _>
    struct equal_impl<Lazy, Lazy, _> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return equal(eval(x), eval(y)); }
    };

    namespace test {
        template <>
        auto instances<Lazy> = tuple(
            type<Functor>,
            type<Applicative>,
            type<Monad>
        );

        template <>
        auto objects<Lazy> = tuple(
            lazy(x<0>), lazy(x<1>), lazy(x<2>)
        );
    }
}}

auto invalid = [](auto x)
{ return x.this_function_must_not_be_instantiated; };


int main() {
    test::check_datatype<Lazy>();

    // Lazy methods
    {
        auto f = test::injection([]{});
        using test::x;

        // lazy
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(),
                lazy(f())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(x<0>),
                lazy(f(x<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(x<0>, x<1>),
                lazy(f(x<0>, x<1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(x<0>, x<1>, x<2>),
                lazy(f(x<0>, x<1>, x<2>))
            ));

            // The function is not applied.
            lazy(invalid)();
            lazy(invalid)(x<0>);
            lazy(invalid)(x<0>, x<1>);
            lazy(invalid)(x<0>, x<1>, x<2>);
        }

        // eval
        {
            BOOST_HANA_CONSTANT_CHECK(equal(eval(lazy(x<0>)), x<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(eval(lazy(x<1>)), x<1>));
        }
    }

    // Functor
    {
        auto x = test::injection([]{})();
        auto f = test::injection([]{});

        // fmap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                fmap(lazy(x), f),
                lazy(f(x))
            ));
        }
    }

    // Applicative
    {
        auto f = test::injection([]{});
        using test::x;

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(x<0>)),
                lazy(f(x<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(x<0>), lazy(x<1>)),
                lazy(f(x<0>, x<1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(x<0>), lazy(x<1>), lazy(x<2>)),
                lazy(f(x<0>, x<1>, x<2>))
            ));

            // The function is not applied.
            ap(lazy(invalid), lazy(x<0>));
            ap(lazy(invalid), lazy(x<0>), lazy(x<1>));
            ap(lazy(invalid), lazy(x<0>), lazy(x<1>), lazy(x<2>));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(lift<Lazy>(x<0>), lazy(x<0>)));
            BOOST_HANA_CONSTANT_CHECK(equal(lift<Lazy>(x<1>), lazy(x<1>)));
        }
    }

    // Monad
    {
        using test::x;
        auto f = compose(lazy, test::injection([]{}));

        // bind
        {
            BOOST_HANA_CONSTANT_CHECK(equal(bind(lazy(x<0>), f), f(x<0>)));
            BOOST_HANA_CONSTANT_CHECK(equal(bind(lazy(x<1>), f), f(x<1>)));
        }

        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(x<0>))),
                lazy(x<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(x<1>))),
                lazy(x<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(lazy(x<1>)))),
                lazy(lazy(x<1>))
            ));
        }

        // Make sure the monadic chain is evaluated in the right order.
        {
            std::array<bool, 3> executed = {{false, false, false}};
            int dummy = 0;

            std::cerr << "creating the monadic chain...\n";
            auto chain = lazy(dummy)
                | [&](int dummy) {
                    std::cerr << "executing the first computation...\n";
                    executed[0] = true;
                    BOOST_HANA_RUNTIME_CHECK(
                        executed == std::array<bool, 3>{{true, false, false}}
                    );
                    return lazy(dummy);
                }
                | [&](int dummy) {
                    std::cerr << "executing the second computation...\n";
                    executed[1] = true;
                    BOOST_HANA_RUNTIME_CHECK(
                        executed == std::array<bool, 3>{{true, true, false}}
                    );
                    return lazy(dummy);
                }
                | [&](int dummy) {
                    std::cerr << "executing the third computation...\n";
                    executed[2] = true;
                    BOOST_HANA_RUNTIME_CHECK(
                        executed == std::array<bool, 3>{{true, true, true}}
                    );
                    return lazy(dummy);
                };

            BOOST_HANA_RUNTIME_CHECK(
                executed == std::array<bool, 3>{{false, false, false}}
            );

            std::cerr << "evaluating the chain...\n";
            eval(chain);
        }
    }
}
