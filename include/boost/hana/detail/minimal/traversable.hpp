/*!
@file
Defines `boost::hana::detail::minimal::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_TRAVERSABLE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_TRAVERSABLE_HPP

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Traversable::traverse_mcd>
    struct Traversable { };

    template <typename X, typename mcd, typename = operators::enable>
    struct traversable_type {
        X value;
        using hana_datatype = Traversable<mcd>;
    };

    template <typename mcd>
    struct make_traversable_impl {
        template <typename X>
        constexpr auto operator()(X x) const
        { return traversable_type<X, mcd>{x}; }
    };

    template <typename mcd = hana::Traversable::traverse_mcd>
    constexpr make_traversable_impl<mcd> traversable{};
}} // end namespace detail::minimal

// Provided for convenience only.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Traversable<mcd>, detail::minimal::Traversable<mcd>
> : Comparable::equal_mcd {
    template <typename T1, typename T2>
    static constexpr auto equal_impl(T1 t1, T2 t2)
    { return equal(t1.value, t2.value); }
};

template <>
struct Traversable::instance<detail::minimal::Traversable<Traversable::traverse_mcd>>
    : Traversable::traverse_mcd
{
    template <typename A, typename F, typename T>
    static constexpr auto traverse_impl(F f, T trav) {
        auto traversable = detail::minimal::traversable<Traversable::traverse_mcd>;
        // this is equivalent to `fmap(traversable, f(trav.value))`
        return ap(lift<A>(traversable), f(trav.value));
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_TRAVERSABLE_HPP
