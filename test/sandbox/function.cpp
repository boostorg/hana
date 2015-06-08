/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/tuple.hpp>

#include <stdexcept>


namespace boost { namespace hana {
    struct Function {
        struct hana {
            struct operators
                : boost::hana::operators::of<Comparable>
            { };
        };
    };

    template <typename Domain, typename Codomain, typename F, typename = operators::adl>
    struct function_type {
        struct hana { using datatype = Function; };

        Domain dom;
        Codomain cod;
        F def;

        friend constexpr auto domain(function_type f)
        { return f.dom; }

        friend constexpr auto codomain(function_type f)
        { return f.cod; }

        template <typename X>
        constexpr auto operator()(X x) const {
            if (!contains(domain(*this), x))
                throw std::domain_error{"use of a hana::function with an argument out of the domain"};
            return def(x);
        }
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto function = [](auto domain, auto codomain) {
        return [=](auto definition) {
            return function_type<decltype(domain), decltype(codomain), decltype(definition)>{
                domain, codomain, definition
            };
        };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto frange = [](auto f) {
        // Note: that would be better handled by a set data structure, but
        // whatever for now.
        return fold.left(transform(domain(f), f), make<Tuple>(), [](auto xs, auto x) {
            return if_(contains(xs, x), xs, prepend(xs, x));
        });
    };


    template <>
    struct equal_impl<Function, Function> {
        template <typename F, typename G>
        static constexpr auto apply(F f, G g) {
            return domain(f) == domain(g) && all_of(domain(f), demux(equal)(f, g));
        }
    };
}} // end namespace boost::hana


// BOOST_HANA_CONSTEXPR_LAMBDA auto is_injective = [](auto f) {
//     auto check = [](auto x, auto y) {
//         return (x != y)     ^implies^   (f(x) != f(y));
//     };
//     return all_of(product(domain(f), domain(f)), check);
// };

// BOOST_HANA_CONSTEXPR_LAMBDA auto is_onto = [](auto f) {
//     return codomain(f) == range(g);
// };


#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    auto f = function(make<Tuple>(1_c, 2_c, 3_c), make<Tuple>(1_c, 2_c, 3_c, 4_c, 5_c, 6_c))(
        [](auto x) { return x + 1_c; }
    );

    auto g = function(make<Tuple>(1_c, 2_c, 3_c), make<Tuple>(2_c, 3_c, 4_c))(
        [](auto x) { return x + 1_c; }
    );

    auto h = function(make<Tuple>(1_c, 2_c, 3_c), make<Tuple>(0_c, 1_c, 2_c))(
        [](auto x) { return x - 1_c; }
    );

    BOOST_HANA_CONSTANT_CHECK(f == g);
    BOOST_HANA_CONSTANT_CHECK(f != h);
    BOOST_HANA_CONSTEXPR_CHECK(f(1) == 2);
    try { f(6); throw; } catch (std::domain_error) { }


    BOOST_HANA_CONSTANT_CHECK(frange(f) == make<Tuple>(4_c, 3_c, 2_c));
    (void)frange;
}
