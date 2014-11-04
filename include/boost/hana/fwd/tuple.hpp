/*!
@file
Forward declares `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TUPLE_HPP
#define BOOST_HANA_FWD_TUPLE_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/iterable.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! General purpose index-based heterogeneous sequence.
    //!
    //! ### Instance of
    //! `Comparable`, `Functor`, `Applicative`, `Monad`, `Traversable`,
    //! `Foldable`, `Iterable`, `List` and `Searchable`.
    struct Tuple {
        struct hana {
            struct enabled_operators : Comparable, Monad, Iterable { };
        };
    };

    //! Create a `Tuple` containing `xs...`.
    //! @relates Tuple
    //!
    //! @todo
    //! - Consider having specialized tuples to hold types, integers, etc...
    //!   It does not fix the problem of partial type classes (e.g. `MplVector`
    //!   is not _actually_ a `List`), but at least we remove `TypeList` and
    //!   `IntegerList`, which are arguably ugly.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto tuple = [](auto&& ...xs) {
        return unspecified-type;
    };
#else
    template <typename ...Xs>
    struct _tuple
        : detail::closure<Xs...>
        , operators::enable_adl
        , operators::Iterable_ops<_tuple<Xs...>>
    {
        using detail::closure<Xs...>::closure; // inherit constructor
        struct hana { using datatype = Tuple; };
    };

    constexpr detail::create<_tuple> tuple{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TUPLE_HPP
