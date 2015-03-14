/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>

#include <array>
#include <iostream>
using namespace boost::hana;


namespace boost { namespace hana {
    // We provide this instance for unit tests only because it is _so_ much
    // more convenient, but this instance is too dangerous for general usage.
    // See the documentation of `Lazy` for more info.
    template <>
    struct equal_impl<Lazy, Lazy> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return equal(eval(x), eval(y)); }
    };
}}

auto invalid = [](auto x)
{ return x.this_function_must_not_be_instantiated; };


int main() {
    test::_injection<0> f{};
    using test::ct_eq;

    auto eqs = make<Tuple>(lazy(ct_eq<0>{}), lazy(ct_eq<1>{}), lazy(ct_eq<2>{}));
    auto eq_elems = make<Tuple>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<1>{});
    auto nested = make<Tuple>(
        lazy(lazy(ct_eq<0>{})),
        lazy(lazy(ct_eq<1>{})),
        lazy(lazy(ct_eq<2>{}))
    );

    //////////////////////////////////////////////////////////////////////////
    // Lazy methods
    //////////////////////////////////////////////////////////////////////////
    {
        // lazy
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(),
                lazy(f())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(ct_eq<0>{}),
                lazy(f(ct_eq<0>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(ct_eq<0>{}, ct_eq<1>{}),
                lazy(f(ct_eq<0>{}, ct_eq<1>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(f)(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                lazy(f(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}))
            ));

            // The function is not applied.
            lazy(invalid)();
            lazy(invalid)(ct_eq<0>{});
            lazy(invalid)(ct_eq<0>{}, ct_eq<1>{});
            lazy(invalid)(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{});
        }

        // eval
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(ct_eq<0>{})),
                ct_eq<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(ct_eq<1>{})),
                ct_eq<1>{}
            ));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        // transform
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(lazy(ct_eq<0>{}), f),
                lazy(f(ct_eq<0>{}))
            ));
        }

        // laws
        test::TestFunctor<Lazy>{eqs, eq_elems};
    }

    // Applicative
    {
        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(ct_eq<0>{})),
                lazy(f(ct_eq<0>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(ct_eq<0>{}), lazy(ct_eq<1>{})),
                lazy(f(ct_eq<0>{}, ct_eq<1>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(lazy(f), lazy(ct_eq<0>{}), lazy(ct_eq<1>{}), lazy(ct_eq<2>{})),
                lazy(f(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}))
            ));

            // The function is not applied.
            ap(lazy(invalid), lazy(ct_eq<0>{}));
            ap(lazy(invalid), lazy(ct_eq<0>{}), lazy(ct_eq<1>{}));
            ap(lazy(invalid), lazy(ct_eq<0>{}), lazy(ct_eq<1>{}), lazy(ct_eq<2>{}));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<Lazy>(ct_eq<0>{}),
                lazy(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<Lazy>(ct_eq<1>{}),
                lazy(ct_eq<1>{})
            ));
        }

        // laws
        test::TestApplicative<Lazy>{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        auto f_ = compose(lazy, f);

        // bind
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                bind(lazy(ct_eq<0>{}), f_),
                f_(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                bind(lazy(ct_eq<1>{}), f_),
                f_(ct_eq<1>{})
            ));
        }

        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(ct_eq<0>{}))),
                lazy(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(ct_eq<1>{}))),
                lazy(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(lazy(lazy(lazy(ct_eq<1>{})))),
                lazy(lazy(ct_eq<1>{}))
            ));
        }

        // laws
        test::TestMonad<Lazy>{eqs, nested};

        //////////////////////////////////////////////////////////////////////////
        // Make sure the monadic chain is evaluated in the right order.
        //////////////////////////////////////////////////////////////////////////
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
