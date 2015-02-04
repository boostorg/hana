/*!
@file
Forward declares `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TUPLE_HPP
#define BOOST_HANA_FWD_TUPLE_HPP

#include <boost/hana/detail/create.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! General purpose index-based heterogeneous sequence with a fixed length.
    //!
    //! `Tuple` is the bread and butter for static metaprogramming.
    //! Conceptually, it is exactly the same as a `std::tuple`; it is just
    //! a sequence able of holding objects of different types and whose size
    //! is fixed at compile-time. However, Hana's tuple provides much more
    //! functionality than its `std` counterpart, and it is also much more
    //! efficient than all standard library implementations tested so far.
    //!
    //! `Tuple`s are index-based sequences. If you need an associative
    //! sequence with a key-based access, then you should consider the
    //! `Map` and `Set` data types instead.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! @todo
    //!
    //! 2. `Functor`\n
    //! @todo
    //!
    //! 3. `Applicative`\n
    //! @todo
    //!
    //! 4. `Monad` (operators provided)\n
    //! @todo
    //!
    //! 5. `MonadPlus`\n
    //! @todo
    //!
    //! 6. `Foldable`\n
    //! @todo
    //!
    //! 7. `Iterable` (operators provided)\n
    //! @todo
    //!
    //! 8. `Searchable`\n
    //! @todo
    //!
    //! 9. `Traversable`\n
    //! @todo
    //!
    //! 10. `Sequence`\n
    //! @todo
    //!
    //! @todo
    //! - Implement Searchable
    //! - Implement Traversable
    //! - Implement `ap`
    //! - Make comparison work with any other Sequence
    struct Tuple { };

    //! Create a `Tuple` containing the given objects.
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
    struct _tuple;

    constexpr detail::create<_tuple> tuple{};
#endif

    //! Create a `Tuple` specialized for holding `Type`s.
    //! @relates Tuple
    //!
    //! This is functionally equivalent to `tuple(type<T>...)`, except that
    //! using `tuple_t` allows the library to perform some compile-time
    //! optimizations. Note that those optimizations are 100% transparent
    //! to the user.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/tuple.cpp tuple_t
    //!
    //! @todo
    //! Implement the optimization.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename ...T>
    constexpr unspecified-type tuple_t{};
#else
    template <typename ...T>
    struct _tuple_t;

    template <typename ...T>
    constexpr typename _tuple_t<T...>::_ tuple_t{};
#endif

    //! Create a `Tuple` specialized for holding `IntegralConstant`s.
    //! @relates Tuple
    //!
    //! This is functionally equivalent to `tuple(integral_constant<T, v>...)`,
    //! except that using `tuple_c` allows the library to perform some
    //! compile-time optimizations. Note that those optimizations are 100%
    //! transparent to the user.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/tuple.cpp tuple_c
    //!
    //! @todo
    //! Implement the optimization.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T, T ...v>
    constexpr unspecified-type tuple_c{};
#else
    template <typename T, T ...v>
    struct _tuple_c;

    template <typename T, T ...v>
    constexpr _tuple_c<T, v...> tuple_c{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TUPLE_HPP
