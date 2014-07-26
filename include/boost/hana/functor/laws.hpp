/*!
@file
Defines `boost::hana::Functor::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_LAWS_HPP
#define BOOST_HANA_FUNCTOR_LAWS_HPP

#include <boost/hana/functor/functor.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Functor::laws {
        template <typename FunctorsOnX, typename FunctionsFromXtoY, typename FunctionsFromYtoZ>
        static constexpr auto check(FunctorsOnX xs, FunctionsFromXtoY fs, FunctionsFromYtoZ gs) {
            return and_(
                all([=](auto x) {
                    return equal(fmap(id, x), x);
                }, xs),
                all([=](auto x) {
                    return all([=](auto f) {
                        return all([=](auto g) {
                            return equal(
                                fmap(compose(f, g), x),
                                fmap(f, fmap(g, x))
                            );
                        }, gs);
                    }, fs);
                }, xs)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_LAWS_HPP
