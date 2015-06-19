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
#include <laws/comonad.hpp>
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
            // With lazy expressions
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(ct_eq<0>{})),
                ct_eq<0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(ct_eq<1>{})),
                ct_eq<1>{}
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(f)()),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(f)(ct_eq<3>{})),
                f(ct_eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(lazy(f)(ct_eq<3>{}, ct_eq<4>{})),
                f(ct_eq<3>{}, ct_eq<4>{})
            ));

            // Should call a nullary function
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval([]{ return ct_eq<3>{}; }),
                ct_eq<3>{}
            ));

            // Should call a unary function with hana::id.
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval([](auto _) { return _(ct_eq<3>{}); }),
                ct_eq<3>{}
            ));

            // For overloaded function objects that are both nullary and unary,
            // the nullary overload should be preferred.
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval(f),
                f()
            ));
        }

        // Make sure this does not move from a destroyed object, as that
        // used to be the case.
        {
            auto x = flatten(lazy(lazy(test::Tracked{1})));
            auto z = eval(x); (void)z;
        }

        // In some cases where a type has a constructor that is way too
        // general, copying a lazy value holding an object of that type
        // could trigger the instantiation of that constructor. If that
        // constructor was ill-formed, the compilation would fail. We
        // make sure this does not happen.
        {
            {
                auto expr = lazy(test::trap_construct{});
                auto implicit_copy = expr;          (void)implicit_copy;
                decltype(expr) explicit_copy(expr); (void)explicit_copy;
            }

            {
                auto expr = lazy(test::trap_construct{})();
                auto implicit_copy = expr;          (void)implicit_copy;
                decltype(expr) explicit_copy(expr); (void)explicit_copy;
            }
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

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
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
        test::TestApplicative<Lazy>{eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        auto f_ = compose(lazy, f);

        // chain
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                chain(lazy(ct_eq<0>{}), f_),
                f_(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                chain(lazy(ct_eq<1>{}), f_),
                f_(ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                lazy(ct_eq<1>{}) | f_,
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
    }

    //////////////////////////////////////////////////////////////////////////
    // Comonad
    //////////////////////////////////////////////////////////////////////////
    {
        // extract
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                extract(lazy(ct_eq<4>{})),
                ct_eq<4>{}
            ));
        }

        // duplicate
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                duplicate(lazy(ct_eq<4>{})),
                lazy(lazy(ct_eq<4>{}))
            ));
        }

        // extend
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                extend(lazy(ct_eq<4>{}), f),
                lazy(f(lazy(ct_eq<4>{})))
            ));
        }

        // laws
        test::TestComonad<Lazy>{eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure the monadic chain is evaluated in the right order.
    //////////////////////////////////////////////////////////////////////////
    {
        std::array<bool, 3> executed = {{false, false, false}};
        int dummy = 0;

        std::cout << "creating the monadic chain...\n";
        auto chain = lazy(dummy)
            | [&](int dummy) {
                std::cout << "executing the first computation...\n";
                executed[0] = true;
                BOOST_HANA_RUNTIME_CHECK(
                    executed == std::array<bool, 3>{{true, false, false}}
                );
                return lazy(dummy);
            }
            | [&](int dummy) {
                std::cout << "executing the second computation...\n";
                executed[1] = true;
                BOOST_HANA_RUNTIME_CHECK(
                    executed == std::array<bool, 3>{{true, true, false}}
                );
                return lazy(dummy);
            }
            | [&](int dummy) {
                std::cout << "executing the third computation...\n";
                executed[2] = true;
                BOOST_HANA_RUNTIME_CHECK(
                    executed == std::array<bool, 3>{{true, true, true}}
                );
                return lazy(dummy);
            };

        BOOST_HANA_RUNTIME_CHECK(
            executed == std::array<bool, 3>{{false, false, false}}
        );

        std::cout << "evaluating the chain...\n";
        eval(chain);
    }
}
