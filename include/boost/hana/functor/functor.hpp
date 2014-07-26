/*!
@file
Forward declares `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_FUNCTOR_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! `Functor` represents types that can be mapped over.
    //!
    //! Instances of `Functor` must satisfy the following laws. For any
    //! `Functor` `x` and functions with matching domains/codomains `f`
    //! and `g`,
    //! @code
    //!     fmap(id, x) == x
    //!     fmap(compose(f, g), x) == fmap(f, fmap(g, x))
    //! @endcode
    struct Functor {
        BOOST_HANA_TYPECLASS(Functor);
        struct fmap_mcd;
        struct adjust_mcd;
        struct laws;
    };

    //! Map `f` over a `Functor`.
    //! @relates Functor
    //!
    //!
    //! @param f
    //! A function called as `f(x)` on each element `x` of the structure,
    //! and returning a new value to replace `x` in the structure.
    //!
    //! @param functor
    //! The structure to map `f` over.
    //!
    //!
    //! ### Example
    //! @snippet example/functor/fmap.cpp to_string
    //!
    //! ### Example
    //! @snippet example/functor/fmap.cpp add_pointer
    BOOST_HANA_CONSTEXPR_LAMBDA auto fmap = [](auto f, auto functor) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::fmap_impl(f, functor);
    };

    //! Apply a function on all the elements of a structure satisfying a
    //! `predicate`.
    //! @relates Functor
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` of the
    //! structure and returning a `Logical` representing whether `f` should
    //! be applied to `x`.
    //!
    //! @param f
    //! A function called as `f(x)` on each element for which the `predicate`
    //! returns a true-valued `Logical`.
    //!
    //! @param functor
    //! The structure to map `f` over.
    //!
    //!
    //! ### Example
    //! @snippet example/functor/adjust.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto adjust = [](auto predicate, auto f, auto functor) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::adjust_impl(predicate, f, functor);
    };

    //! Replace all the elements of a structure satisfying a `predicate`
    //! with a fixed value.
    //! @relates Functor
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` of the
    //! structure and returning a `Logical` representing whether `x` should
    //! be replaced by `value`.
    //!
    //! @param value
    //! A value by which every element `x` of the structure for which
    //! `predicate` returns a true-valued `Logical` is replaced.
    //!
    //! @param functor
    //! The structure to replace elements of.
    //!
    //!
    //! ### Example
    //! @snippet example/functor/replace.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto replace = [](auto predicate, auto value, auto functor) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::replace_impl(predicate, value, functor);
    };

    //! Replace all the elements of a structure with a fixed value.
    //! @relates Functor
    //!
    //!
    //! @param value
    //! A value by which every element `x` of the structure is replaced,
    //! unconditionally.
    //!
    //! @param functor
    //! The structure to fill with `value`.
    //!
    //!
    //! ### Example
    //! @snippet example/functor/fill.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto fill = [](auto value, auto functor) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::fill_impl(value, functor);
    };

    namespace functor_detail {
        struct common {
            template <typename Pred, typename Value, typename F>
            static constexpr auto replace_impl(Pred pred, Value v, F functor)
            { return adjust(pred, [=](auto) { return v; }, functor); }

            template <typename Value, typename F>
            static constexpr auto fill_impl(Value v, F functor)
            { return fmap([=](auto) { return v; }, functor); }
        };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_FUNCTOR_HPP
