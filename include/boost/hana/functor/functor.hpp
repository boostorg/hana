/*!
@file
Forward declares `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_FUNCTOR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Functor` represents types that can be mapped over.
    //!
    //! Instances of `Functor` must satisfy the following laws. For any
    //! `Functor` `x` and functions with matching domains/codomains `f`
    //! and `g`,
    //! @code
    //!     fmap(x, id) == x
    //!     fmap(x, compose(f, g)) == fmap(fmap(x, g), f)
    //! @endcode
    struct Functor {
        BOOST_HANA_TYPECLASS(Functor);
        struct fmap_mcd;
        struct adjust_mcd;
        template <typename T>
        struct list_mcd;
    };

    //! Map a function over a `Functor`.
    //! @relates Functor
    //!
    //!
    //! @param functor
    //! The structure to map `f` over.
    //!
    //! @param f
    //! A function called as `f(x)` on each element `x` of the structure,
    //! and returning a new value to replace `x` in the structure.
    //!
    //!
    //! ### Example
    //! @snippet example/functor.cpp fmap
    //!
    //! ### Benchmarks
    //! @image html benchmark/functor/fmap.ctime.png
    constexpr auto fmap = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& functor, auto&& f) -> decltype(auto) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::fmap_impl(
            detail::std::forward<decltype(functor)>(functor),
            detail::std::forward<decltype(f)>(f)
        );
    };

    //! Apply a function on all the elements of a structure satisfying a
    //! `predicate`.
    //! @relates Functor
    //!
    //!
    //! @param functor
    //! The structure to map `f` over.
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
    //!
    //! ### Example
    //! @snippet example/functor.cpp adjust
    //!
    //! ### Benchmarks
    //! @image html benchmark/functor/adjust.ctime.png
    constexpr auto adjust = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& functor, auto&& predicate, auto&& f) -> decltype(auto) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::adjust_impl(
            detail::std::forward<decltype(functor)>(functor),
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(f)>(f)
        );
    };

    //! Replace all the elements of a structure satisfying a `predicate`
    //! with a fixed value.
    //! @relates Functor
    //!
    //!
    //! @param functor
    //! The structure to replace elements of.
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
    //!
    //! ### Example
    //! @snippet example/functor.cpp replace
    //!
    //! ### Benchmarks
    //! @image html benchmark/functor/replace.ctime.png
    constexpr auto replace = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& functor, auto&& predicate, auto&& value) -> decltype(auto) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::replace_impl(
            detail::std::forward<decltype(functor)>(functor),
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(value)>(value)
        );
    };

    //! Replace all the elements of a structure with a fixed value.
    //! @relates Functor
    //!
    //!
    //! @param functor
    //! The structure to fill with `value`.
    //!
    //! @param value
    //! A value by which every element `x` of the structure is replaced,
    //! unconditionally.
    //!
    //!
    //! ### Example
    //! @snippet example/functor.cpp fill
    //!
    //! ### Benchmarks
    //! @image html benchmark/functor/fill.ctime.png
    constexpr auto fill = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto&& functor, auto&& value) -> decltype(auto) {
        return Functor::instance<
            datatype_t<decltype(functor)>
        >::fill_impl(
            detail::std::forward<decltype(functor)>(functor),
            detail::std::forward<decltype(value)>(value)
        );
    };

    namespace functor_detail {
        struct common {
            template <typename F, typename Pred, typename Value>
            static constexpr auto replace_impl(F functor, Pred pred, Value v)
            { return adjust(functor, pred, [=](auto) { return v; }); }

            template <typename F, typename Value>
            static constexpr auto fill_impl(F functor, Value v)
            { return fmap(functor, [=](auto) { return v; }); }
        };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_FUNCTOR_HPP
