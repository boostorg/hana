/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>

#include <stdexcept>


namespace boost { namespace hana {
    struct Function;

    template <typename Domain, typename Codomain, typename F,
        typename = operators<Comparable>
    >
    struct function_type {
        using hana_datatype = Function;

        Domain dom;
        Codomain cod;
        F def;

        friend constexpr auto domain(function_type f)
        { return f.dom; }

        friend constexpr auto codomain(function_type f)
        { return f.cod; }

        template <typename X>
        constexpr auto operator()(X x) const {
            if (!elem(domain(*this), x))
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
        return foldl(fmap(f, domain(f)), list(), [](auto xs, auto x) {
            return if_(elem(xs, x), xs, cons(x, xs));
        });
    };


    template <>
    struct Comparable::instance<Function, Function> : Comparable::equal_mcd {
        template <typename F, typename G>
        static constexpr auto equal_impl(F f, G g) {
            return domain(f) == domain(g) && all(domain(f), demux(equal, f, g));
        }
    };
}} // end namespace boost::hana


// BOOST_HANA_CONSTEXPR_LAMBDA auto is_injective = [](auto f) {
//     auto check = [](auto x, auto y) {
//         return (x != y)     ^implies^   (f(x) != f(y));
//     };
//     return all(product(domain(f), domain(f)), check);
// };

// BOOST_HANA_CONSTEXPR_LAMBDA auto is_onto = [](auto f) {
//     return codomain(f) == range(g);
// };


#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/range.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    auto f = function(list(1_c, 2_c, 3_c), list(1_c, 2_c, 3_c, 4_c, 5_c, 6_c))(
        [](auto x) { return x + 1_c; }
    );

    auto g = function(list(1_c, 2_c, 3_c), list(2_c, 3_c, 4_c))(
        [](auto x) { return x + 1_c; }
    );

    auto h = function(list(1_c, 2_c, 3_c), list(0_c, 1_c, 2_c))(
        [](auto x) { return x - 1_c; }
    );

    BOOST_HANA_CONSTANT_ASSERT(f == g);
    BOOST_HANA_CONSTANT_ASSERT(f != h);
    BOOST_HANA_CONSTEXPR_ASSERT(f(1) == 2);
    try { f(6); throw; } catch (std::domain_error) { }


    BOOST_HANA_CONSTANT_ASSERT(frange(f) == list(4_c, 3_c, 2_c));
    (void)frange;
}
