/*!
@file
Adapts Boost.MPL vectors.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Data type representing Boost.MPL vectors.
    //!
    //! ## Instance of
    //! `Functor`, `Iterable`, `List` and `Comparable`
    //!
    //! ## MPL / Hana cheat sheet
    //! This section presents correspondences between operations with the
    //! Boost.MPL and operations with Boost.Hana. The organization follows
    //! that of the Boost.MPL algorithms.
    //!
    //! ### Iteration algorithms
    //! #### `fold`
    //! @snippet example/ext/boost/mpl/vector/cheatsheet/fold.cpp mpl
    //! @snippet example/ext/boost/mpl/vector/cheatsheet/fold.cpp hana
    //!
    //! @todo
    //! - Finish the cheat sheet.
    //! - Remove the tuple.hpp include
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    struct MplVector { };
#else
    using MplVector = boost::mpl::sequence_tag<boost::mpl::vector<>>::type;
#endif

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename boost::mpl::sequence_tag<T>::type,
            MplVector
        >::value
    >> {
        using type = MplVector;
    };

    //! `Functor` instance for Boost.MPL vectors.
    //!
    //! Because Boost.MPL vectors can only hold types, only functions on
    //! `Type`s may be mapped over a vector. Hence, MPL vectors are only
    //! partial `Functor`s.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/functor.cpp
    //!
    //! @todo
    //! In the case where `f` is not a metafunction class, don't use
    //! `Tuple` to do the mapping.
    template <>
    struct Functor::instance<MplVector> : Functor::fmap_mcd {
        template <bool mfc, typename Dummy = void>
        struct helper {
            template <typename f, typename xs>
            static constexpr auto apply(f, xs) {
                return typename boost::mpl::transform<xs, f>::type{};
            }
        };

        template <typename Dummy>
        struct helper<false, Dummy> {
            template <typename F, typename Xs>
            static constexpr auto apply(F f, Xs xs) {
                return fmap(f, to<Tuple>(xs));
            }
        };

        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            return helper<detail::is_metafunction_class<F>::value>::apply(f, xs);
        }
    };

    //! `Iterable` instance for Boost.MPL vectors.
    //!
    //! That instance is equivalent to `TypeList`'s instance of `Iterable`.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/iterable.cpp
    template <>
    struct Iterable::instance<MplVector> : Iterable::mcd {
        template <typename xs>
        static constexpr auto head_impl(xs)
        { return type<typename boost::mpl::front<xs>::type>; }

        template <typename xs>
        static constexpr auto tail_impl(xs)
        { return typename boost::mpl::pop_front<xs>::type{}; }

        template <typename xs>
        static constexpr auto is_empty_impl(xs)
        { return bool_<boost::mpl::empty<xs>::type::value>; }
    };

    //! `List` instance for Boost.MPL vectors.
    //!
    //! Note that since Boost.MPL vectors can only hold types,
    //! only `Type`s can be used with `cons`.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/list.cpp
    template <>
    struct List::instance<MplVector> : List::mcd<MplVector> {
        template <typename x, typename xs>
        static constexpr auto cons_impl(x, xs) {
            static_assert(detail::std::is_same<datatype_t<x>, Type>::value,
            "Only Types may be prepended to a Boost.MPL vector.");

            return typename boost::mpl::push_front<
                xs, typename x::type
            >::type{};
        }

        static constexpr auto nil_impl()
        { return boost::mpl::vector<>{}; }
    };

    //! `Comparable` instance for Boost.MPL vectors.
    //!
    //! Comparing two MPL vectors is equivalent to using `boost::mpl::equal`.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/comparable.cpp
    template <>
    struct Comparable::instance<MplVector, MplVector> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys) {
            return bool_<boost::mpl::equal<Xs, Ys>::type::value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
