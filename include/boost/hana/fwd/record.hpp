/*!
@file
Forward declares `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_RECORD_HPP
#define BOOST_HANA_FWD_RECORD_HPP

#include <boost/hana/core/method.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Represents record-like user-defined types.
    //!
    //! A record-like type is any type which is fundamentally a `struct`,
    //! regardless of the implementation of its accessors and other similar
    //! details. Hence, `Record`s can be seen as `Map`s to which keys can't
    //! be added.
    //!
    //! Instances of `Record` are created by specifying an ordered mapping
    //! from keys to accessors, where keys are objects playing the role of
    //! member names, and accessors allow individual members to be retrieved
    //! from an object. Specifically, an accessor `A` for a subobject `sub`
    //! of type `T` inside an object of type `Obj` is a function such that
    //! @code
    //!     A(Obj&)       -> T&
    //!     A(Obj const&) -> T const&
    //!     A(Obj&&)      -> T
    //! @endcode
    //!
    //! where `F(X) -> Y` means that the return type of `F` when called with
    //! an object of type `X` is exactly `Y`. Moreover, `A` may only access
    //! and/or modify the subobject `sub`. This restriction exists so that
    //! @code
    //!     Obj obj = {...};
    //!     auto subobject_1 = accessor_1(std::move(obj));
    //!     auto subobject_2 = accessor_2(std::move(obj));
    //! @endcode
    //!
    //! is valid code which extracts two subobjects of `obj` while (perhaps)
    //! moving from them. If `accessor_1` was allowed to modify arbitrary
    //! subobjects of `obj`, we could end up accessing a moved-from object
    //! with `accessor_2` (if `accessor_1` had moved from it). Similarly,
    //! if `accessor_2` was allowed to access arbitrary subobjects of `obj`,
    //! it could end up accessing the moved-from subobject that was moved-from
    //! with `accessor_1`.
    //!
    //! @note
    //! The @ref BOOST_HANA_DEFINE_RECORD and
    //! @ref BOOST_HANA_DEFINE_RECORD_INTRUSIVE macros can
    //! also be used to define instances of `Record`.
    //!
    //! @todo
    //! The restrictions on what constitutes an accessor are pretty fierce,
    //! and using `std::move` on the same object twice feels completely wrong.
    //! Is there a better way to allow an object to be decomposed optimally
    //! into its subobjects without resorting to such hacks?
    //!
    //! ### Example
    //! @include example/record/howto.cpp
    struct Record { };

    //! Returns a list of pairs representing the data structure.
    //! @relates Record
    //!
    //! Specifically, `members<R>()` is a `List` of `Product`s associating keys
    //! to functions, where a pair `(k, f)` means that the member represented
    //! by the key `k` can be accessed by calling the function `f` on an object
    //! of data type `R`.
    //!
    //! ### Example
    //! @snippet example/record.cpp members
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto members = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_METHOD(members_impl);

    template <typename R>
    struct _members {
        constexpr decltype(auto) operator()() const
        { return dispatch<members_impl<R>>::apply(); }
    };

    template <typename R>
    constexpr _members<R> members{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_RECORD_HPP
