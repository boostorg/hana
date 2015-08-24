/*!
@file
Defines `boost::hana::Product`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_PRODUCT_HPP
#define BOOST_HANA_CONCEPT_PRODUCT_HPP

#include <boost/hana/fwd/concept/product.hpp>

#include <boost/hana/and.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/or.hpp>

#include <boost/hana/concept/foldable.hpp> //! @todo Remove this include

#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename P>
    struct models_impl<Product, P> {
        static constexpr bool value = !is_default<first_impl<P>>::value &&
                                      !is_default<second_impl<P>>::value;
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct equal_impl<T, U, when<
        _models<Product, T>::value && _models<Product, U>::value
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return hana::and_(
                hana::equal(hana::first(x), hana::first(y)),
                hana::equal(hana::second(x), hana::second(y))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<T, U, when<
        _models<Product, T>::value && _models<Product, U>::value
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return hana::or_(
                hana::less(hana::first(x), hana::first(y)),
                hana::and_(
                    hana::less_equal(hana::first(x), hana::first(y)),
                    hana::less(hana::second(x), hana::second(y))
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct unpack_impl<T, when<_models<Product, T>::value>> {
        template <typename P, typename F>
        static constexpr decltype(auto) apply(P&& p, F&& f) {
            return static_cast<F&&>(f)(
                hana::first(static_cast<P&&>(p)),
                hana::second(static_cast<P&&>(p))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_PRODUCT_HPP
