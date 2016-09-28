/*!
@file
Forward declares `boost::hana::view`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_VIEW_HPP
#define BOOST_HANA_FWD_VIEW_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/make.hpp>

#include <cstddef>


BOOST_HANA_NAMESPACE_BEGIN
    //! @ingroup group-datatypes
    //! Lazy view over an existing object.
    //!
    //! It is sometimes desirable to manipulate an object as-if some algorithm
    //! had been applied on it, but without eagerly applying the algorithm on
    //! the original object. This can be done by employing a view, which allows
    //! creating a modified _image_ of the object. This works by having views
    //! implement the interface required by many concepts in a lazy way,
    //! deferring the actual computation to some key places in the interface.
    //! For example, for a view over a sequence, the computation of most
    //! algorithms will be deferred to the time when an element of the view
    //! is accessed.
    //!
    //! Views have the following advantages:
    //! - No temporary container created between algorithms
    //! - Lazy, so only the minimum amount of computation is done
    //!
    //! They also have the following disadvantages:
    //! - Reference semantics open the door to dangling references and other
    //!   lifetime bugs; be careful when using views
    //! - Fetching subobjects of a view multiple times can cause some
    //!   computations to be done multiple times. For example, accessing
    //!   the first member of a `hana::transform`ed view twice will cause
    //!   the transforming function to be applied twice on the fetched element.
    //!   This is not a problem of views per se, but of naive lazy evaluation
    //!   in general.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! In theory, it ought to be possible to create general views of objects
    //! that model almost arbitrary concepts. However, in the current version
    //! of the library, only views over `Sequence`s are supported. When a view
    //! is created over a `Sequence`, the following models are provided for the
    //! resulting view:
    //! - `Foldable`
    //! - `Iterable`
    //! - `Functor`
    //! - `Applicative`
    //! - `Monad`
    //! - `MonadPlus`
    //! - `Comparable`
    //! - `Orderable`
    //!
    //! Example
    //! -------
    //! @include example/view/view.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename implementation_defined>
    struct view {
        //! Copy-construct a view from another view, such that both views
        //! now refer to the same underlying object.
        constexpr view(view const& other) = default;

#if 0   //! @todo Implement these operators
        //! Equivalent to `hana::chain`.
        template <typename Ys, typename F>
        friend constexpr auto operator|(view<Ys>, F);

        //! Equivalent to `hana::equal`
        template <typename X, typename Y>
        friend constexpr auto operator==(X&& x, Y&& y);

        //! Equivalent to `hana::not_equal`
        template <typename X, typename Y>
        friend constexpr auto operator!=(X&& x, Y&& y);

        //! Equivalent to `hana::less`
        template <typename X, typename Y>
        friend constexpr auto operator<(X&& x, Y&& y);

        //! Equivalent to `hana::greater`
        template <typename X, typename Y>
        friend constexpr auto operator>(X&& x, Y&& y);

        //! Equivalent to `hana::less_equal`
        template <typename X, typename Y>
        friend constexpr auto operator<=(X&& x, Y&& y);

        //! Equivalent to `hana::greater_equal`
        template <typename X, typename Y>
        friend constexpr auto operator>=(X&& x, Y&& y);

        //! Equivalent to `hana::at`
        template <typename N>
        constexpr decltype(auto) operator[](N&& n);
#endif
    };
#else
    namespace experimental { template <typename Xs> struct identity_view_t; }
    template <typename Xs>
    using view = experimental::identity_view_t<Xs>;
#endif

    //! Tag representing `hana::view`s.
    //! @related view
    struct view_tag { };


#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Function object for creating a `view`.
    //! @relates hana::view
    //!
    //! Given an object `xs`, `make<view_tag>` returns a new view over that
    //! object. The created view only refers to the original object, and no
    //! copy is made. The lifetime of the referred-to object must span the
    //! entire lifetime of the view, otherwise the view is dangling and the
    //! behavior is undefined.
    //!
    //! Example
    //! -------
    //! @include example/view/make.cpp
    template <>
    constexpr auto make<view_tag> = [](auto& xs) {
        return view<implementation_defined>{xs};
    };
#endif

    //! Alias to `make<view_tag>`; provided for convenience.
    //! @relates hana::view
    constexpr auto make_view = make<view_tag>;
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_VIEW_HPP
