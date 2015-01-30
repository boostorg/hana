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


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! `List` containing `Type`s only.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! @todo
    //!
    //! 2. `Foldable`\n
    //! @todo
    //!
    //! 3. `Iterable` (operators provided)\n
    //! @todo
    //!
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
    struct TypeList {
#ifndef BOOST_HANA_DOXYGEN_INVOKED
        struct hana {
            struct enabled_operators
                : Comparable
            { };
        };
#endif
    };

    //! Creates a list containing the given types.
    //! @relates TypeList
    //!
    //! This is functionally equivalent to `list(type<xs>...)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type_list.cpp type_list
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename ...T>
    constexpr unspecified-dependent-type type_list{};
#else
    template <typename ...xs>
    struct _type_list {
        struct hidden : _type_list<xs...>, operators::enable_adl {
            struct hana { using datatype = TypeList; };
        };
    };

    template <typename ...xs>
    constexpr typename _type_list<xs...>::hidden type_list{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TYPE_LIST_HPP
