/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct x0; struct x1; struct x2;

namespace test_values {
    constexpr struct {
        //! @todo Decouple this from `Type`.
        constexpr auto operator()(operators::_type<x0>) const { return 0; }
        constexpr auto operator()(operators::_type<x1>) const { return 1; }
        constexpr auto operator()(operators::_type<x2>) const { return 2; }
    } f{};

    BOOST_HANA_CONSTEXPR_LAMBDA auto g = _ + int_<1>;

    void go() {
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<>) == list());
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0>) == list(0));
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0, x1>) == list(0, 1));
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0, x1, x2>) == list(0, 1, 2));

        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<>, g, f));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0>, g, f));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0, x1>, g, f));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0, x1, x2>, g, f));
    }
}

namespace test_types {
    template <typename ...> struct F { };
    template <typename ...> struct G { };
    constexpr auto f = template_<F>;
    constexpr auto g = template_<G>;

    void go() {
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<>) == list_t<>);
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0>) == list_t<F<x0>>);
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0, x1>) == list_t<F<x0>, F<x1>>);
        BOOST_HANA_STATIC_ASSERT(fmap(f, list_t<x0, x1, x2>) == list_t<F<x0>, F<x1>, F<x2>>);

        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<>, f, g));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0>, f, g));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0, x1>, f, g));
        BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(list_t<x0, x1, x2>, f, g));
    }
}


int main() {
    test_values::go();
    test_types::go();
}
