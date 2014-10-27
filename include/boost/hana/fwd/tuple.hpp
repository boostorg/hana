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
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
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
        struct hana_enabled_operators
            : Comparable, Monad, Iterable
        { };
    };

    namespace detail { namespace repr {
        template <typename Storage>
        struct tuple
            : operators::enable_adl,
              operators::Iterable_ops<tuple<Storage>>
        {
            explicit constexpr tuple(Storage&& s)
                : storage(detail::std::move(s))
            { }

            struct hana { using datatype = Tuple; };
            Storage storage;
        };
    }}

    //! Create a `Tuple` containing `xs...`.
    //! @relates Tuple
    //!
    //! @todo
    //! - Consider having specialized tuples to hold types, integers, etc...
    //!   It does not fix the problem of partial type classes (e.g. `MplVector`
    //!   is not _actually_ a `List`), but at least we remove `TypeList` and
    //!   `IntegerList`, which are arguably ugly.
    //! - Use perfect forwarding to construct the inner lambda capture when
    //!   this is supported and this bug is resolved: http://llvm.org/bugs/show_bug.cgi?id=20939
    //! - Enable the test in tuple/tuple.cpp once the above is resolved.
    BOOST_HANA_CONSTEXPR_LAMBDA auto tuple = [](auto ...xs) -> decltype(auto) {
        auto storage = [=](auto f) -> decltype(auto) { return f(xs...); };
        return detail::repr::tuple<decltype(storage)>{detail::std::move(storage)};
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TUPLE_HPP
