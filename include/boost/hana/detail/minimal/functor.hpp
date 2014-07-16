/*!
@file
Defines `boost::hana::detail::minimal::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_FUNCTOR_HPP
#define BOOST_HANA_DETAIL_MINIMAL_FUNCTOR_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/functor/adjust_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Functor::fmap_mcd>
    struct Functor { };

    template <typename T, typename mcd, typename = operators::enable>
    struct functor_type {
        T value;
        using hana_datatype = Functor<mcd>;
    };

    template <typename mcd>
    struct make_functor_impl {
        template <typename X>
        constexpr auto operator()(X x) const
        { return functor_type<X, mcd>{x}; }
    };

    template <typename mcd = hana::Functor::fmap_mcd>
    constexpr make_functor_impl<mcd> functor{};
}} // end namespace detail::minimal

// Since some Functor methods return a Functor, we define the obvious
// Comparable instance so we can perform comparisons in the unit tests.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Functor<mcd>, detail::minimal::Functor<mcd>
>
    : Comparable::equal_mcd
{
    template <typename F1, typename F2>
    static constexpr auto equal_impl(F1 x, F2 y)
    { return equal(x.value, y.value); }
};

template <>
struct Functor::instance<detail::minimal::Functor<Functor::fmap_mcd>>
    : Functor::fmap_mcd
{
    template <typename F, typename Fn>
    static constexpr auto fmap_impl(F f, Fn self) {
        return detail::minimal::functor<Functor::fmap_mcd>(f(self.value));
    }
};

template <>
struct Functor::instance<detail::minimal::Functor<Functor::adjust_mcd>>
    : Functor::adjust_mcd
{
    template <typename Pred, typename F, typename Fn>
    static constexpr auto adjust_impl(Pred p, F f, Fn self) {
        auto x = self.value;
        return detail::minimal::functor<Functor::adjust_mcd>(p(x) ? f(x) : x);
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_FUNCTOR_HPP
