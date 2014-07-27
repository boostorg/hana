/*!
@file
Forward declares `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_PAIR_HPP
#define BOOST_HANA_PAIR_PAIR_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! @ingroup datatypes
    //! Generic container of two elements.
    //!
    //! `Pair` represents a container of two elements with means to extract
    //! each of them separately. Since the instance for `Pair` is unique up
    //! to isomorphism, `Pair` is also a data type instantiating the `Pair`
    //! type class with the only possible instance up to isomorphism.
    //!
    //! @todo
    //! Provide and document instances for `Functor`, `Applicative`,
    //! `Monad` and `Foldable`.
    struct Pair {
        BOOST_HANA_TYPECLASS(Pair);
        struct mcd;
    };

    //! Return the first element of a pair.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair/first.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto first = [](auto pair) {
        return Pair::instance<
            datatype_t<decltype(pair)>
        >::first_impl(pair);
    };

    //! Return the second element of a pair.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair/second.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto second = [](auto pair) {
        return Pair::instance<
            datatype_t<decltype(pair)>
        >::second_impl(pair);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_PAIR_HPP
