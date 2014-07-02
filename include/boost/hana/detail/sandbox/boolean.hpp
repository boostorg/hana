/*!
@file
Defines `boost::hana::Boolean`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOLEAN_HPP
#define BOOST_HANA_BOOLEAN_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    BOOST_HANA_TYPECLASS_BOILERPLATE(struct Boolean)

    //! @ingroup typeclasses
    //! The `Boolean` type class defines a
    //! [boolean algebra](http://en.wikipedia.org/wiki/Boolean_algebra_(structure)).
    //!
    //! @note
    //! We could define what it means for a Boolean to be compile-time and
    //! what it means to be runtime. Then, it might be possible to specify
    //! some algorithms in term of the runtime-ness of the predicates they
    //! use internally. Shooting from the hip, compile-time ness might be
    //! the capacity to have a `then` and an `else_` branch of different
    //! type, plus the insurance to never try to cast the unused branch
    //! (that could work hand-in-hand with a `lazy` feature).
    //!
    //! ----------------------------------------------------------------------
    //!
    //! ## Laws
    //! ...
    struct Boolean : typeclass<Boolean> {
        struct mcd { };
    };

    //! ...
    //! @method{Boolean}
    BOOST_HANA_CONSTEXPR_LAMBDA auto complement = [](auto x) {
        return Boolean::instance<datatype_t<decltype(x)>>::complement_impl(x);
    };

    //! ...
    //! @method{Boolean}
    BOOST_HANA_CONSTEXPR_LAMBDA auto meet = [](auto x, auto y) {
        return Boolean::instance<datatype_t<decltype(x)>>::meet_impl(x, y);
    };

    //! ...
    //! @method{Boolean}
    BOOST_HANA_CONSTEXPR_LAMBDA auto join = [](auto x, auto y) {
        return Boolean::instance<datatype_t<decltype(x)>>::join_impl(x, y);
    };

    //! ...
    //! @method{Boolean}
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto top = Boolean::instance<T>::top_impl();

    //! ...
    //! @method{Boolean}
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto bottom = Boolean::instance<T>::bottom_impl();
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOLEAN_HPP
