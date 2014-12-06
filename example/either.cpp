/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>

#include <string>
using namespace boost::hana;


int main() {
    {
        //! [comparable]
        BOOST_HANA_CONSTEXPR_CHECK(left('x') == left('x'));
        BOOST_HANA_CONSTANT_CHECK(right('x') != left('x'));
        BOOST_HANA_CONSTEXPR_CHECK(right('x') == right('x'));
        BOOST_HANA_CONSTEXPR_CHECK(right('x') != right('y'));
        //! [comparable]
    }

    {
        //! [functor]
        auto safediv = infix([](auto x, auto y) {
            return eval_if(y == int_<0>,
                always(left(std::string{"division by zero"})),
                [=](auto _) { return right(x / _(y)); }
            );
        });

        auto inc = partial(plus, int_<1>);
        BOOST_HANA_CONSTANT_CHECK(
            fmap(int_<6> ^safediv^ int_<3>, inc) == right(int_<3>)
        );

        BOOST_HANA_RUNTIME_CHECK(
            fmap(int_<6> ^safediv^ int_<0>, inc) == left("division by zero")
        );
        //! [functor]
    }

    {
        //! [monad]
        auto safe_div = [](auto x, auto y) {
            return eval_if(y == int_<0>,
                always(left(std::string{"division by zero"})),
                [=](auto _) { return right(x / _(y)); }
            );
        };

        auto safe_dec = [](auto x) {
            return eval_if(x == int_<0>,
                always(left(std::string{"negative value"})),
                [=](auto _) { return right(_(x) - int_<1>); }
            );
        };

        BOOST_HANA_RUNTIME_CHECK(
            (safe_div(int_<4>, int_<0>) | safe_dec) == left("division by zero")
        );

        BOOST_HANA_RUNTIME_CHECK(
            (safe_div(int_<0>, int_<2>) | safe_dec) == left("negative value")
        );

        BOOST_HANA_CONSTANT_CHECK(
            (safe_div(int_<4>, int_<2>) | safe_dec) == right(int_<1>)
        );
        //! [monad]
    }

    {
        //! [either]
        BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + 1; };
        BOOST_HANA_CONSTEXPR_LAMBDA auto dec = [](auto x) { return x - 1; };
        BOOST_HANA_CONSTEXPR_CHECK(either(inc, dec, left(1)) == 2);
        BOOST_HANA_CONSTEXPR_CHECK(either(inc, dec, right(1)) == 0);
        //! [either]
    }

    {
        //! [left]
        BOOST_HANA_CONSTEXPR_LAMBDA auto left_value = left('x');
        //! [left]
        (void)left_value;
    }

    {
        //! [right]
        BOOST_HANA_CONSTEXPR_LAMBDA auto right_value = right('x');
        //! [right]
        (void)right_value;
    }
}
