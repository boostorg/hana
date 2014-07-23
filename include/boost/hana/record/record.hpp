/*!
@file
Forward declares `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_RECORD_HPP
#define BOOST_HANA_RECORD_RECORD_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Type class for record-like user-defined types.
    //!
    //! A record-like type is any type which is fundamentally a `struct`,
    //! regardless of the implementation of its accessors and other similar
    //! details.
    //!
    //! ### Example
    //! @include example/record/howto.cpp
    struct Record {
        BOOST_HANA_TYPECLASS(Record);
        struct mcd;
    };

    //! A list of pairs representing the data structure.
    //! @method{Record}
    //!
    //! Specifically, `members<R>` is a `List` of `Pair`s associating keys to
    //! functions, where a pair `(k, f)` means that the member represented by
    //! the key `k` can be accessed by calling the function `f` on an object
    //! of data type `R`.
    template <typename R>
    BOOST_HANA_CONSTEXPR_LAMBDA auto members = Record::instance<R>::members_impl();
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_RECORD_HPP
