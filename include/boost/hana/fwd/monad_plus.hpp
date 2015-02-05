/*!
@file
Forward declares `boost::hana::MonadPlus`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONAD_PLUS_HPP
#define BOOST_HANA_FWD_MONAD_PLUS_HPP

namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `MonadPlus` concept represents ...
    //!
    //! ...
    //!
    //!
    //! Laws
    //! ----
    //! ...
    //!
    //!
    //! Superclasses
    //! ------------
    //! `Functor`, `Applicative` and `Monad`
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `concat` and `nil`\n
    //! ...
    //!
    struct MonadPlus { };

    //! ...
    //! @relates MonadPlus
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto concat = [](auto&& xs, auto&& ys) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct concat_impl;

    struct _concat {
        template <typename Xs, typename Ys>
        constexpr decltype(auto) operator()(Xs&& xs, Ys&& ys) const {
            static_assert(detail::std::is_same<
                typename datatype<Xs>::type, typename datatype<Ys>::type
            >{},
            "boost::hana::concat: both arguments must have the same data type");
            return concat_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys)
            );
        }
    };

    constexpr _concat concat{};
#endif

    //! `nil<L>()` is an empty list of data type `L`.
    //! @relates MonadPlus
    //!
    //! @tparam L
    //! The data type of the empty list wanted. This must be an instance of
    //! the `List` type class.
    //!
    //!
    //! ### Example
    //! @snippet example/monad_plus.cpp nil
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto nil = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct nil_impl;

    template <typename M>
    struct _nil {
        constexpr decltype(auto) operator()() const {
            return nil_impl<M>::apply();
        }
    };

    template <typename M>
    constexpr _nil<M> nil{};
#endif

    //! Prepend an element to the head of a list.
    //! @relates MonadPlus
    //!
    //!
    //! @param x
    //! An element to prepend to the list.
    //!
    //! @param xs
    //! The list to which an element is prepended.
    //!
    //!
    //! ### Example
    //! @snippet example/monad_plus.cpp cons
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto prepend = [](auto&& x, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct prepend_impl;

    struct _prepend {
        template <typename X, typename Xs>
        constexpr decltype(auto) operator()(X&& x, Xs&& xs) const {
            return prepend_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _prepend prepend{};
#endif

    //! Append an element to the end of a list.
    //! @relates List
    //!
    //!
    //! @param xs
    //! The list to which an element is appended.
    //!
    //! @param x
    //! An element to append to the list.
    //!
    //!
    //! ### Example
    //! @snippet example/monad_plus.cpp snoc
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto append = [](auto&& xs, auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct append_impl;

    struct _append {
        template <typename Xs, typename X>
        constexpr decltype(auto) operator()(Xs&& xs, X&& x) const {
            return append_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<X>(x)
            );
        }
    };

    constexpr _append append{};
#endif

    //! Return a list containing only the elements satisfying a `predicate`.
    //! @relates MonadPlus
    //!
    //!
    //! @param xs
    //! The list to filter.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the list
    //! and returning a `Logical` representing whether that element should be
    //! __kept__ in the resulting list. In the current version of the library,
    //! the `predicate` has to return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //!
    //! ### Example
    //! @snippet example/monad_plus.cpp filter
    //!
    //! ### Benchmarks
    //! @image html benchmark/monad_plus/filter.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto filter = [](auto&& xs, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct filter_impl;

    struct _filter {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
            return filter_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _filter filter{};
#endif

    //! ...
    //! @relates MonadPlus
    //!
    //!
    //!
    //! @tparam M
    //! ...
    //!
    //! @param n
    //! An `IntegralConstant` representing the number of copies of `x` to
    //! shove into the returned list.
    //!
    //! @param x
    //! The value to fill the list with.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/monad_plus.cpp cycle
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto cycle = [](auto&& n, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct cycle_impl;

    struct _cycle {
        template <typename N, typename Xs>
        constexpr decltype(auto) operator()(N&& n, Xs&& xs) const {
            return cycle_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<N>(n),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _cycle cycle{};
#endif

    //! Create a list containing `n` copies of a value.
    //! @relates MonadPlus
    //!
    //! Specifically, `repeat<L>(n, x)` is a list of data type `L` containing
    //! `n` copies of `x`.
    //!
    //!
    //! @tparam L
    //! The data type of the list to create. It must be an instance of `List`.
    //!
    //! @param n
    //! An `IntegralConstant` representing the number of copies of `x` to
    //! shove into the returned list.
    //!
    //! @param x
    //! The value to fill the list with.
    //!
    //!
    //! ### Example
    //! @snippet example/monad_plus.cpp repeat
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto repeat = [](auto&& n, auto&& x) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct repeat_impl;

    template <typename M>
    struct _repeat {
        template <typename N, typename X>
        constexpr decltype(auto) operator()(N&& n, X&& x) const {
            return repeat_impl<M>::apply(
                detail::std::forward<decltype(n)>(n),
                detail::std::forward<decltype(x)>(x)
            );
        }
    };

    template <typename M>
    constexpr _repeat<M> repeat{};
#endif

    //! ...
    //! @relates MonadPlus
    //!
    //! ...
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto prefix = [](auto&& pfx, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct prefix_impl;

    struct _prefix {
        template <typename Pfx, typename Xs>
        constexpr decltype(auto) operator()(Pfx&& pfx, Xs&& xs) const {
            return prefix_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pfx>(pfx),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _prefix prefix{};
#endif

    //! ...
    //! @relates MonadPlus
    //!
    //! ...
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto suffix = [](auto&& pfx, auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename M, typename = void>
    struct suffix_impl;

    struct _suffix {
        template <typename Pfx, typename Xs>
        constexpr decltype(auto) operator()(Pfx&& pfx, Xs&& xs) const {
            return suffix_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pfx>(pfx),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _suffix suffix{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONAD_PLUS_HPP
