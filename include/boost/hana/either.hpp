/*!
@file
Defines `boost::hana::Either`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_HPP
#define BOOST_HANA_EITHER_HPP

#include <boost/hana/fwd/either.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>

// instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Either`s.
    //!
    //! Two `Either`s are equal if and only if they both contain left values
    //! or they both contain right values and those values are equal.
    //!
    //! ### Example
    //! @snippet example/either.cpp comparable
    template <>
    struct Comparable::instance<Either, Either> : Comparable::equal_mcd {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) equal_impl(E<T> const& x, E<U> const& y) {
            return equal(x.value, y.value);
        }

        template <template <typename ...> class E1,
                  template <typename ...> class E2, typename T, typename U>
        static constexpr auto equal_impl(E1<T> const&, E2<U> const&)
        { return false_; }
    };

    //! Instance of `Functor` for `Either`s.
    //!
    //! Since `Either` can contain one of two possible values of different
    //! data types and `fmap` accepts a single function, `Either`'s instance
    //! of `Functor` can only map the function over one arbitrarily-defined
    //! side of the `Either`. Hence, mapping a function over an `Either e`
    //! does nothing if `e` contains a left value, and it applies the function
    //! if `e` contains a right value. In other words:
    //! @code
    //!     fmap(left(x), f) == left(x)
    //!     fmap(right(x), f) == right(f(x))
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/either.cpp functor
    template <>
    struct Functor::instance<Either> : Functor::fmap_mcd {
        template <typename E, typename F>
        static constexpr decltype(auto) fmap_impl(E&& e, F&& f) {
            return either(left,
                compose(right, detail::std::forward<F>(f)),
                detail::std::forward<E>(e)
            );
        }
    };

    //! Instance of `Applicative` for `Either`.
    //!
    //! The instance of `Applicative` for `Either` follows naturally from
    //! the instance of `Functor`. Specifically,
    //! @code
    //!     ap(left(x), anything) == left(x)
    //!     ap(right(x), left(anything)) == right(x)
    //!     ap(right(f), right(x)) == right(f(x))
    //!     lift<Either>(x) == right(x)
    //! @endcode
    //!
    //! ### Example
    //! @include example/either/applicative.cpp
    template <>
    struct Applicative::instance<Either> : Applicative::mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return right(detail::std::forward<X>(x)); }

        template <typename E, typename X>
        static constexpr decltype(auto) ap_impl(E&& e, X&& x) {
            return either(left,
                partial(fmap, detail::std::forward<X>(x)),
                detail::std::forward<E>(e)
            );
        }
    };

    //! Instance of `Monad` for `Either`.
    //!
    //! The instance of `Monad` for `Either` follows naturally from
    //! the instance of `Applicative`. Specifically,
    //! @code
    //!     flatten(right(right(x))) == right(x)
    //!     flatten(anything else) == anything else
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/either.cpp monad
    template <>
    struct Monad::instance<Either> : Monad::flatten_mcd<Either> {
        template <typename E>
        static constexpr decltype(auto) flatten_impl(E&& e) {
            return either(left, id, detail::std::forward<E>(e));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
