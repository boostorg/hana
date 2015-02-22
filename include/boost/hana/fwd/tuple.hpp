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
    //! `Sequence`, and all its superclasses
    //!
    //!
    //! Provided operators
    //! ------------------
    //! For convenience, operators for methods of the following concepts can
    //! be used with `Tuple`: `Comparable`, `Orderable`, `Monad`, `Iterable`.
    struct Tuple { };

    //! Create a `Tuple` containing the given objects.
    //! @relates Tuple
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
    //! This is functionally equivalent to `make<Tuple>(type<T>...)`, except
    //! that using `tuple_t` allows the library to perform some compile-time
    //! optimizations. Also note that the type of the objects returned by
    //! `tuple_t` and an equivalent call to `make<Tuple>` may differ.
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
    //! This is functionally equivalent to `make<Tuple>(integral_constant<T, v>...)`,
    //! except that using `tuple_c` allows the library to perform some
    //! compile-time optimizations. Also note that the type of the objects
    //! returned by `tuple_c` and an equivalent call to `make<Tuple>` may differ.
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
