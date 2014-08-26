/*!
@file
Defines the instance of `boost::hana::Functor` for `boost::hana::Either`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_FUNCTOR_HPP
#define BOOST_HANA_EITHER_FUNCTOR_HPP

#include <boost/hana/either/either.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Functor` for `Either`s.
    //!
    //! Since `Either` can contain one of two possible values of different
    //! data types and `fmap` accepts a single function, `Either`'s instance
    //! of `Functor` can only map the function over one arbitrarily-defined
    //! side of the `Either`. Hence, mapping a function over an `Either e`
    //! does nothing if `e` contains a left value, and it applies the function
    //! if `e` contains a right value. In other words:
    //! @code
    //!     fmap(f, left(x)) == left(x)
    //!     fmap(f, right(x)) == right(f(x))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/either.cpp functor
    template <>
    struct Functor::instance<Either> : Functor::fmap_mcd {
        template <typename F, typename X>
        static constexpr auto fmap_impl(F f, either_detail::right<X> x)
        { return right(f(x.value)); }

        template <typename F, typename E>
        static constexpr auto fmap_impl(F f, E e)
        { return e; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_FUNCTOR_HPP
