/*!
@file
Defines type class instances for foreign data types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_HPP
#define BOOST_HANA_FOREIGN_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/orderable.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for objects of foreign types.
    //!
    //! Any two foreign objects whose types can be compared using
    //! `operator==` are automatically instances of `Comparable` by
    //! using that comparison.
    template <typename T, typename U>
    struct Comparable::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() == detail::std::declval<U>())
    >>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) == detail::std::forward<Y>(y);
        }
    };

    //! Instance of `Enumerable` for objects of foreign numeric types.
    //!
    //! Any foreign object that can be incremented using `operator++` and
    //! decremented using `operator--` is `Enumerable` using those operations
    //! for `succ` and `pred` respectively.
    template <typename T>
    struct Enumerable::instance<T, when_valid<
        decltype(++detail::std::declval<T&>()),
        decltype(--detail::std::declval<T&>())
    >>
        : Enumerable::mcd
    {
        template <typename X>
        static constexpr auto succ_impl(X /* by value */ x)
        { return ++x; }

        template <typename X>
        static constexpr auto pred_impl(X /* by value */ x)
        { return --x; }
    };

    //! Instance of `Foldable` for array types.
    //!
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples.
    //!
    //! @note
    //! Builtin arrays can't be made more than `Foldable` (e.g. `Iterable`)
    //! because they can't be empty and they also can't be returned from
    //! functions.
    template <typename T, detail::std::size_t N>
    struct Foldable::instance<T[N]>
        : Foldable::unpack_mcd
    {
        template <typename Xs, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, detail::std::index_sequence<i...>) {
            return detail::std::forward<F>(f)(
                detail::std::forward<T>(xs[i])...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return unpack_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };

    //! Instance of `Group` for objects of foreign numeric types.
    //!
    //! Any two foreign objects forming a `Monoid` and that can be
    //! subtracted with the usual `operator-` naturally form an additive
    //! group, with the group subtraction being that usual `operator-`.
    template <typename T, typename U>
    struct Group::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() - detail::std::declval<U>()),
        char[are<Monoid, T, U>()]
    >>
        : Group::minus_mcd<T, U>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) minus_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) - detail::std::forward<Y>(y);
        }
    };

    //! Instance of `IntegralDomain` for objects of foreign numeric types.
    //!
    //! Any two foreign objects that are `Rings`s, that can be divided
    //! and moded with the usual operators (`/` and `%`) naturally form
    //! an integral domain with those operations.
    template <typename T, typename U>
    struct IntegralDomain::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() / detail::std::declval<U>()),
        decltype(detail::std::declval<T>() % detail::std::declval<U>()),
        char[are<Ring, T, U>()]
    >> : IntegralDomain::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) quot_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) / detail::std::forward<Y>(y);
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) mod_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) % detail::std::forward<Y>(y);
        }
    };

    //! Instance of `Logical` for objects of foreign types that can be
    //! implicitly converted to `bool`.
    //!
    //! Any foreign object that can be converted to `bool` implicitly is an
    //! instance of `Logical` by converting that object to `bool` and then
    //! using the obvious instance for `bool`.
    //!
    //! @bug
    //! We can't use perfect forwarding because of this bug:
    //! http://llvm.org/bugs/show_bug.cgi?id=20619
    template <typename L>
    struct Logical::instance<L, when_valid<
        decltype(detail::std::declval<L>() ? void() : void())
    >>
        : Logical::mcd
    {
        template <typename T, typename E>
        static constexpr auto eval_if_impl(bool cond, T t, E e) {
            return cond ? t(id) : e(id);
        }

        static constexpr bool not_impl(bool cond)
        { return !cond; }
    };

    //! Instance of `Monoid` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that can be added with the usual `operator+`
    //! and for which a valid conversion from `int` exists (for both)
    //! naturally form an additive `Monoid`, with `0` being the identity
    //! and the usual `operator+` being the associative operation.
    template <typename T, typename U>
    struct Monoid::instance<T, U, when_valid<
        decltype(static_cast<T>(0)),
        decltype(static_cast<U>(0)),
        decltype(detail::std::declval<T>() + detail::std::declval<U>())
    >> : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) plus_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) + detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr decltype(auto) zero_impl()
        { return static_cast<T>(0); }
    };

    //! Instance of `Orderable` for foreign but `LessThanComparable` objects.
    //!
    //! Any two foreign objects whose types can be compared using `operator<`
    //! are automatically instances of `Orderable` by using that comparison.
    template <typename T, typename U>
    struct Orderable::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() < detail::std::declval<U>())
    >>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) < detail::std::forward<Y>(y);
        }
    };

    //! Instance of `Ring` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that are `Group`s, that can be multiplied
    //! with the usual `operator*` and for which a valid conversion from `int`
    //! exists (for both) naturally form a multiplicative `Ring`, with `1`
    //! being the identity and the usual `operator*` being the ring operation.
    template <typename T, typename U>
    struct Ring::instance<T, U, when_valid<
        decltype(static_cast<T>(1)),
        decltype(static_cast<U>(1)),
        decltype(detail::std::declval<T>() * detail::std::declval<U>()),
        char[are<Group, T, U>()]
    >> : Ring::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) mult_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) * detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr auto one_impl()
        { return static_cast<T>(1); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_HPP
