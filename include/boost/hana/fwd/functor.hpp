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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Functor` represents types that can be mapped over.
    //!
    //!
    //! ### Laws
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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fmap = [](auto&& functor, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct fmap_impl : fmap_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct fmap_impl<T, when<condition>> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return Functor::instance<T>::fmap_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    struct _fmap {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return fmap_impl<datatype_t<Xs>>::apply(
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
    template <typename T, typename = void>
    struct adjust_impl : adjust_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct adjust_impl<T, when<condition>> {
        template <typename Xs, typename Pred, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred, F&& f) {
            return Functor::instance<T>::adjust_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<F>(f)
            );
        }
    };

    struct _adjust {
        template <typename Xs, typename Pred, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, F&& f) const {
            return adjust_impl<datatype_t<Xs>>::apply(
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
    template <typename T, typename = void>
    struct replace_impl : replace_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct replace_impl<T, when<condition>> {
        template <typename Xs, typename Pred, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred, Value&& value) {
            return Functor::instance<T>::replace_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<Value>(value)
            );
        }
    };

    struct _replace {
        template <typename Xs, typename Pred, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, Value&& value) const {
            return replace_impl<datatype_t<Xs>>::apply(
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
    template <typename T, typename = void>
    struct fill_impl : fill_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct fill_impl<T, when<condition>> {
        template <typename Xs, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Value&& value) {
            return Functor::instance<T>::fill_impl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value)
            );
        }
    };

    struct _fill {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
            return fill_impl<datatype_t<Xs>>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _fill fill{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FUNCTOR_HPP
