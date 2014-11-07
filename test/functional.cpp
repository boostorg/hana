/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>

#include <utility>
using namespace boost::hana;


template <int i>
struct _nonpod { virtual ~_nonpod() { } };

template <int i = 0>
_nonpod<i> nonpod{};

template <int i>
struct _undefined { };

template <int i = 0>
constexpr _undefined<i> undefined{};

struct move_only {
    move_only(move_only&&) = default;
    move_only(move_only const&) = delete;
};


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto h = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto i = test::injection([]{});
    using test::x;

    // always
    {
        auto z = x<0>;
        BOOST_HANA_CONSTANT_ASSERT(equal(
            always(z)(), z
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            always(z)(undefined<1>), z
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            always(z)(undefined<1>, undefined<2>), z
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            always(z)(undefined<1>, undefined<2>, undefined<3>), z
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            always(z)(undefined<1>, undefined<2>, undefined<3>, undefined<4>), z
        ));

        always(z)(nonpod<>);
        auto m = always(move_only{})(undefined<1>); (void)m;
    }

    // apply
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(apply(f), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal(apply(f, x<0>), f(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(apply(f, x<0>, x<1>), f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(apply(f, x<0>, x<1>, x<2>), f(x<0>, x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(apply(f, x<0>, x<1>, x<2>, x<3>), f(x<0>, x<1>, x<2>, x<3>)));
        apply(f, nonpod<>);
    }

    // arg
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<1>(x<1>),
            x<1>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<1>(x<1>, undefined<2>),
            x<1>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<1>(x<1>, undefined<2>, undefined<3>),
            x<1>
        ));
        arg<1>(nonpod<1>);
        arg<1>(nonpod<1>, nonpod<2>);


        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<2>(undefined<1>, x<2>),
            x<2>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<2>(undefined<1>, x<2>, undefined<3>),
            x<2>
        ));
        arg<2>(nonpod<1>, nonpod<2>);
        arg<2>(nonpod<1>, nonpod<2>, nonpod<3>);


        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<3>(undefined<1>, undefined<2>, x<3>),
            x<3>
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            arg<3>(undefined<1>, undefined<2>, x<3>, undefined<4>),
            x<3>
        ));
        arg<3>(nonpod<1>, nonpod<2>, nonpod<3>);
        arg<3>(nonpod<1>, nonpod<2>, nonpod<3>, nonpod<4>);
    }

    // compose
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g)(x<0>),
            f(g(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g)(x<0>, x<1>),
            f(g(x<0>), x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g)(x<0>, x<1>, x<2>),
            f(g(x<0>), x<1>, x<2>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g, h)(x<0>),
            f(g(h(x<0>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g, h)(x<0>, x<1>),
            f(g(h(x<0>)), x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            compose(f, g, h)(x<0>, x<1>, x<2>),
            f(g(h(x<0>)), x<1>, x<2>)
        ));

        auto h = [capture = move_only{}](int dummy) { return 1; };
        auto i = [](int dummy) { return 1; };
        compose(std::move(h), i)(1);

        {
            // Compose move-only functions.
            auto f = [capture = move_only{}] (int) { return 1; };
            auto g = [capture = move_only{}] (int) { return 1; };
            auto c = compose(std::move(f), std::move(g)); (void)c;
        }
    }

    // curry
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<0>(f)(), f()));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<1>(f)(x<1>), f(x<1>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<2>(f)(x<1>)(x<2>), f(x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<2>(f)(x<1>, x<2>), f(x<1>, x<2>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(f)(x<1>)(x<2>)(x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(f)(x<1>)(x<2>, x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(f)(x<1>, x<2>)(x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(f)(x<1>, x<2>, x<3>), f(x<1>, x<2>, x<3>)));


        // Make sure curry is idempotent; this is important because it allows
        // currying a function in generic contexts where it is unknown whether
        // the function is already curried.
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<0>(curry<0>(f))(), f()));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<1>(curry<1>(f))(x<1>), f(x<1>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<2>(curry<2>(f))(x<1>)(x<2>), f(x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<2>(curry<2>(f))(x<1>, x<2>), f(x<1>, x<2>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(curry<3>(f))(x<1>)(x<2>)(x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(curry<3>(f))(x<1>)(x<2>, x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(curry<3>(f))(x<1>, x<2>)(x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(curry<3>(curry<3>(f))(x<1>, x<2>, x<3>), f(x<1>, x<2>, x<3>)));
    }

    // demux
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)()(), f()));

        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g)(x<1>), f(g(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g)(x<1>, x<2>), f(g(x<1>, x<2>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g)(x<1>, x<2>, x<3>), f(g(x<1>, x<2>, x<3>))));

        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g, h)(x<1>), f(g(x<1>), h(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g, h)(x<1>, x<2>), f(g(x<1>, x<2>), h(x<1>, x<2>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(demux(f)(g, h)(x<1>, x<2>, x<3>), f(g(x<1>, x<2>, x<3>), h(x<1>, x<2>, x<3>))));
    }

    // fix (tested separately)
    {

    }

    // flip
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(flip(f)(x<1>, x<2>), f(x<2>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(flip(f)(x<1>, x<2>, x<3>), f(x<2>, x<1>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(flip(f)(x<1>, x<2>, x<3>, x<4>), f(x<2>, x<1>, x<3>, x<4>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(flip(f)(x<1>, x<2>, x<3>, x<4>, x<5>), f(x<2>, x<1>, x<3>, x<4>, x<5>)));
    }

    // id
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(id(x<0>), x<0>));
        BOOST_HANA_CONSTANT_ASSERT(equal(id(x<1>), x<1>));

        (void)id(move_only{});
    }

    // lockstep
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(lockstep(f)()(), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal(lockstep(f)(g)(x<1>), f(g(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(lockstep(f)(g, h)(x<1>, x<2>), f(g(x<1>), h(x<2>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(lockstep(f)(g, h, i)(x<1>, x<2>, x<3>), f(g(x<1>), h(x<2>), i(x<3>))));
    }

    // infix
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = infix(f);

        // disregard associativity
        BOOST_HANA_CONSTANT_ASSERT(equal(x<0> ^g^ x<1>, f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal((x<0> ^g)^ x<1>, f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(x<0> ^(g^ x<1>), f(x<0>, x<1>)));

        // left partial application
        BOOST_HANA_CONSTANT_ASSERT(equal((x<0>^g)(x<1>), f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal((x<0>^g)(x<1>, x<2>), f(x<0>, x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal((x<0>^g)(x<1>, x<2>, x<3>), f(x<0>, x<1>, x<2>, x<3>)));

        // right partial application
        BOOST_HANA_CONSTANT_ASSERT(equal((g^x<1>)(x<0>), f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal((g^x<2>)(x<0>, x<1>), f(x<0>, x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal((g^x<3>)(x<0>, x<1>, x<2>), f(x<0>, x<1>, x<2>, x<3>)));

        // equivalence with the base function
        BOOST_HANA_CONSTANT_ASSERT(equal(g(x<0>, x<1>), f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(g(x<0>, x<1>, x<2>), f(x<0>, x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(g(x<0>, x<1>, x<2>, x<3>), f(x<0>, x<1>, x<2>, x<3>)));
    }

    // on
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(x<0>), f(g(x<0>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(x<0>, x<1>), f(g(x<0>), g(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(x<0>, x<1>, x<2>), f(g(x<0>), g(x<1>), g(x<2>))));
        BOOST_HANA_CONSTANT_ASSERT(equal(on(f, g)(x<0>, x<1>, x<2>, x<3>), f(g(x<0>), g(x<1>), g(x<2>), g(x<3>))));

        // check the infix version
        BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(x<0>), f(g(x<0>))));
        BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(x<0>, x<1>), f(g(x<0>), g(x<1>))));
        BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(x<0>, x<1>, x<2>), f(g(x<0>), g(x<1>), g(x<2>))));
        BOOST_HANA_CONSTANT_ASSERT(equal((f ^on^ g)(x<0>, x<1>, x<2>, x<3>), f(g(x<0>), g(x<1>), g(x<2>), g(x<3>))));
    }

    // overload
    {
        // 1 function
        {
            auto f = overload([](int) { return x<1>; });
            BOOST_HANA_CONSTANT_ASSERT(equal(f(int{}), x<1>));
        }

        // 2 functions
        {
            auto f = overload(
                [](int) { return x<1>; },
                [](float) { return x<2>; }
            );
            BOOST_HANA_CONSTANT_ASSERT(equal(f(int{}), x<1>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(float{}), x<2>));
        }

        // 3 functions
        {
            auto f = overload(
                [](int) { return x<1>; },
                [](float) { return x<2>; },
                static_cast<decltype(x<3>)(*)(char)>([](char) { return x<3>; })
            );
            BOOST_HANA_CONSTANT_ASSERT(equal(f(int{}), x<1>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(float{}), x<2>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(char{}), x<3>));
        }

        // 4 functions
        {
            auto f = overload(
                [](int) { return x<1>; },
                [](float) { return x<2>; },
                static_cast<decltype(x<3>)(*)(char)>([](char) { return x<3>; }),
                [](auto) { return x<4>; }
            );

            struct otherwise { };
            BOOST_HANA_CONSTANT_ASSERT(equal(f(int{}), x<1>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(float{}), x<2>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(char{}), x<3>));
            BOOST_HANA_CONSTANT_ASSERT(equal(f(otherwise{}), x<4>));
        }

        // check move-only friendlyness for bare functions
        {
            void (*g)(move_only) = [](move_only) { };
            overload(g)(move_only{});
        }

        // check non-strict matches which require a conversion
        {
            struct convertible_to_int { operator int() const { return 1; } };
            auto f = [](int) { return x<0>; };

            BOOST_HANA_CONSTANT_ASSERT(equal(
                overload(f)(convertible_to_int{}),
                x<0>
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                overload(static_cast<decltype(x<0>)(*)(int)>(f))(convertible_to_int{}),
                x<0>
            ));
        }
    }

    // partial
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f)(), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f)(x<1>), f(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f)(x<1>, x<2>), f(x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f)(x<1>, x<2>, x<3>), f(x<1>, x<2>, x<3>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>)(), f(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>)(x<2>), f(x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>)(x<2>, x<3>), f(x<1>, x<2>, x<3>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>, x<2>)(), f(x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>, x<2>)(x<3>), f(x<1>, x<2>, x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>, x<2>)(x<3>, x<4>), f(x<1>, x<2>, x<3>, x<4>)));

        BOOST_HANA_CONSTANT_ASSERT(equal(partial(f, x<1>, x<2>, x<3>)(), f(x<1>, x<2>, x<3>)));
    }

    // placeholder (tested separately)
    {

    }
}
