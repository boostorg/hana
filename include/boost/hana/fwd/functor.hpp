/*!
@file
Forward declares `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FUNCTOR_HPP
#define BOOST_HANA_FWD_FUNCTOR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Functor` represents types that can be mapped over.
    //!
    //!
    //! Laws
    //! ----
    //! Instances of `Functor` must satisfy the following laws. For any
    //! `Functor` `xs`,
    //! @code
    //!     fmap(xs, id) == xs
    //!     fmap(xs, compose(f, g)) == fmap(fmap(xs, g), f)
    //!
    //!     adjust(xs, pred, f) == fmap(xs, [](auto x) {
    //!         if(pred(x)) return f(x);
    //!         else        return x;
    //!     })
    //!
    //!     replace(xs, pred, v) == adjust(xs, pred, always(v))
    //!
    //!     fill(xs, v) == replace(xs, always(true_), v)
    //! @endcode
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! 1. `fmap`
    //! @todo
    //!
    //! 2. `adjust`
    //! @todo
    //!
    struct Functor { };

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fmap = [](auto&& functor, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(fmap_impl);

    struct _fmap {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return dispatch<fmap_impl<typename datatype<Xs>::type>>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _fmap fmap{};
#endif

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto adjust = [](auto&& functor, auto&& predicate, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(adjust_impl);

    struct _adjust {
        template <typename Xs, typename Pred, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, F&& f) const {
            return dispatch<adjust_impl<typename datatype<Xs>::type>>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _adjust adjust{};
#endif

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto replace = [](auto&& functor, auto&& predicate, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(replace_impl);

    struct _replace {
        template <typename Xs, typename Pred, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, Value&& value) const {
            return dispatch<replace_impl<typename datatype<Xs>::type>>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _replace replace{};
#endif

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fill = [](auto&& functor, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(fill_impl);

    struct _fill {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
            return dispatch<fill_impl<typename datatype<Xs>::type>>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _fill fill{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FUNCTOR_HPP
