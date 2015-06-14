/*!
@file
Defines `boost::hana::ext::boost::mpl::Vector`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/vector.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace mpl {
        //! @ingroup group-ext
        //! Adapter for Boost.MPL vectors.
        //!
        //!
        //! Modeled concepts
        //! ----------------
        //! It is possible for MPL vectors to model a couple of concepts.
        //! However, because they are only able to hold types, they lack
        //! the generality required to model concepts like `Functor`,
        //! `Sequence` and other related concepts.
        //!
        //! 1. `Comparable`\n
        //! Two MPL vectors are equal if and only if they contain the same
        //! number of types, and if all those types are equal.
        //! @snippet example/ext/boost/mpl/vector.cpp Comparable
        //!
        //! 2. `Foldable`\n
        //! Folding a MPL vector is equivalent to folding it as a `Sequence`.
        //! @snippet example/ext/boost/mpl/vector.cpp Foldable
        //!
        //! 3. `Iterable`\n
        //! Iterating over a MPL vector is just iterating over each of the
        //! types it contains, as if it were a `Sequence`.
        //! @snippet example/ext/boost/mpl/vector.cpp Iterable
        //!
        //! 4. `Searchable`\n
        //! A MPL vector can be searched as if it were a `Tuple` of `Type`s.
        //! @snippet example/ext/boost/mpl/vector.cpp Searchable
        //!
        //!
        //! Conversion from any `Foldable`
        //! ------------------------------
        //! A MPL vector can be created from any `Foldable`. More precisely,
        //! for a `Foldable` `xs` whose linearization is `[x1, ..., xn]`,
        //! @code
        //!     to<ext::boost::mpl::Vector>(xs) == mpl::vector<t1, ..., tn>
        //! @endcode
        //! where `tk` is the type of `xk`, or the type contained in `xk` if
        //! `xk` is a `Type`.
        //! @warning
        //! The limitations on the size of `mpl::vector`s are inherited by
        //! this conversion utility, and hence trying to convert a `Foldable`
        //! containing more than `BOOST_MPL_MAX_VECTOR_SIZE` elements is an
        //! error.
        //! @snippet example/ext/boost/mpl/vector.cpp from_Foldable
#ifdef BOOST_HANA_DOXYGEN_INVOKED
        struct Vector { };
#else
        using Vector = ::boost::mpl::sequence_tag< ::boost::mpl::vector<>>::type;
#endif
    }}}

    template <typename T>
    struct datatype<T, when<
        std::is_same<
            typename ::boost::mpl::sequence_tag<T>::type,
            ::boost::mpl::sequence_tag< ::boost::mpl::vector<>>::type
        >::value
    >> {
        using type = ext::boost::mpl::Vector;
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::boost::mpl::Vector, ext::boost::mpl::Vector> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys) {
            return typename ::boost::mpl::equal<Xs, Ys>::type{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct fold_left_impl<ext::boost::mpl::Vector>
        : Iterable::fold_left_impl<ext::boost::mpl::Vector>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return type<typename ::boost::mpl::front<xs>::type>; }
    };

    template <>
    struct tail_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return typename ::boost::mpl::pop_front<xs>::type{}; }
    };

    template <>
    struct is_empty_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return typename ::boost::mpl::empty<xs>::type{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<ext::boost::mpl::Vector>
        : Iterable::find_if_impl<ext::boost::mpl::Vector>
    { };

    template <>
    struct any_of_impl<ext::boost::mpl::Vector>
        : Iterable::any_of_impl<ext::boost::mpl::Vector>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Conversion from a Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<ext::boost::mpl::Vector, F, when<_models<Foldable, F>{}()>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            auto vector_type = hana::unpack(static_cast<Xs&&>(xs),
                                    hana::template_<::boost::mpl::vector>);
            return typename decltype(vector_type)::type{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
