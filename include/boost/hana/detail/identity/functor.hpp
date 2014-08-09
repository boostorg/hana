/*!
@file
Defines the `boost::hana::Functor` instance for `boost::hana::detail::Identity`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_IDENTITY_FUNCTOR_HPP
#define BOOST_HANA_DETAIL_IDENTITY_FUNCTOR_HPP

#include <boost/hana/detail/identity/identity.hpp>

#include <boost/hana/functor/adjust_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Functor::instance<detail::Identity<Mcd>>
        : Functor::fmap_mcd
    {
        template <typename F, typename Id>
        static constexpr auto fmap_impl(F f, Id self) {
            return self.wrap(f(self.value));
        }
    };

    template <>
    struct Functor::instance<detail::Identity<Functor::adjust_mcd>>
        : Functor::adjust_mcd
    {
        template <typename P, typename F, typename Id>
        static constexpr auto adjust_impl(P p, F f, Id self) {
            auto x = eval_if(p(self.value),
                [=](auto _) { return _(f)(self.value); },
                [=](auto _) { return self.value; }
            );
            return self.wrap(x);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_IDENTITY_FUNCTOR_HPP
