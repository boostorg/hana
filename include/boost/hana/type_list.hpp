/*!
@file
Defines `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_HPP
#define BOOST_HANA_TYPE_LIST_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! `List` containing `Type`s only.
    //!
    //! @note
    //! `TypeList` may be more efficient than `List` because of
    //! optimizations. When possible, it should be preferred.
    //!
    //! @todo
    //! - Efficient membership testing is possible.
    //! - `TypeList` is not really a `Functor` because the function must
    //!   map from `Type`s to `Type`s. Should it be modified so that
    //!   `TypeList` becomes `List` if we try to store something else
    //!   than `Type`s? The same issue goes for `IntegerList`.
    struct TypeList { };

    namespace detail { namespace repr {
        template <typename ...xs>
        struct type_list {
            struct _ : operators::enable {
                using hana_datatype = TypeList;
                using storage = type_list<xs...>;
            };
        };
    }}

    //! Creates a list containing the given types.
    //! @relates TypeList
    //!
    //! This is functionally equivalent to `list(type<xs>...)`.
    //!
    //! ### Example
    //! @snippet example/type_list/type_list.cpp main
    template <typename ...xs>
    constexpr typename detail::repr::type_list<xs...>::_ type_list{};

    template <>
    struct Iterable::instance<TypeList> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs)
        { return head_impl(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto head_impl(detail::repr::type_list<X, Xs...>)
        { return type<X>; }


        template <typename Xs>
        static constexpr auto tail_impl(Xs)
        { return tail_impl(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto tail_impl(detail::repr::type_list<X, Xs...>)
        { return type_list<Xs...>; }


        template <typename Xs>
        static constexpr auto is_empty_impl(Xs)
        { return is_empty_impl(typename Xs::storage{}); }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(detail::repr::type_list<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct List::instance<TypeList> : List::mcd<TypeList> {
        template <typename X, typename ...Xs>
        static constexpr auto cons_impl(X, detail::repr::type_list<Xs...>)
        { return type_list<typename X::type, Xs...>; }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return cons_impl(x, typename Xs::storage{}); }

        static constexpr auto nil_impl()
        { return type_list<>; }
    };

    template <>
    struct Foldable::instance<TypeList> : Foldable::unpack_mcd {
        //! @todo Fix the lost optimization caused by unpacking with `Type`s.
        template <typename ...Xs, typename F>
        static constexpr auto unpack_impl(detail::repr::type_list<Xs...>, F f)
        { return f(type<Xs>...); }

        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs, F f)
        { return unpack_impl(typename Xs::storage{}, f); }
    };

    template <>
    struct Comparable::instance<TypeList, TypeList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys)
        { return false_; }

        template <typename Xs>
        static constexpr auto equal_impl(Xs, Xs)
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_HPP
