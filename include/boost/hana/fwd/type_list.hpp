/*!
@file
Forward declares `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TYPE_LIST_HPP
#define BOOST_HANA_FWD_TYPE_LIST_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! `List` containing `Type`s only.
    //!
    //! ### Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`, `Iterable`,
    //! `Foldable`, `List`, `Traversable` and `Searchable`
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
    struct TypeList { struct hana_enabled_operators : Comparable, Monad { }; };

    namespace detail { namespace repr {
        template <typename ...xs>
        struct type_list {
            struct hidden : operators::enable_adl {
                struct hana { using datatype = TypeList; };
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
    constexpr typename detail::repr::type_list<xs...>::hidden type_list{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TYPE_LIST_HPP
