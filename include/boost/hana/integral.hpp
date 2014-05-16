/*!
 * @file
 * Defines `boost::hana::Integral`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_HPP
#define BOOST_HANA_INTEGRAL_HPP

#include <cstddef>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename T, T t>
    struct Integral {
        constexpr operator T() const { return t; }

        constexpr Integral<decltype(!t), !t> operator!() const
        { return {}; }
    };

#define BOOST_HANA_INTEGRAL_BINARY_OP(op)                                   \
    template <typename U, U u, typename V, V v>                             \
    constexpr Integral<decltype(u op v), (u op v)>                          \
    operator op(Integral<U, u>, Integral<V, v>)                             \
    { return {}; }                                                          \
/**/
    BOOST_HANA_INTEGRAL_BINARY_OP(+)
    BOOST_HANA_INTEGRAL_BINARY_OP(-)
    BOOST_HANA_INTEGRAL_BINARY_OP(*)
    BOOST_HANA_INTEGRAL_BINARY_OP(/)

    BOOST_HANA_INTEGRAL_BINARY_OP(==)
    BOOST_HANA_INTEGRAL_BINARY_OP(!=)

    BOOST_HANA_INTEGRAL_BINARY_OP(<)
    BOOST_HANA_INTEGRAL_BINARY_OP(<=)
    BOOST_HANA_INTEGRAL_BINARY_OP(>)
    BOOST_HANA_INTEGRAL_BINARY_OP(>=)

    BOOST_HANA_INTEGRAL_BINARY_OP(||)
    BOOST_HANA_INTEGRAL_BINARY_OP(&&)
#undef BOOST_HANA_INTEGRAL_BINARY_OP

    template <int i> using Int = Integral<int, i>;
    template <int i> constexpr Int<i> int_{};

    template <bool b> using Bool = Integral<bool, b>;
    template <bool b> constexpr Bool<b> bool_{};
    constexpr auto true_ = bool_<true>;
    constexpr auto false_ = bool_<false>;

    template <std::size_t i> using SizeT = Integral<std::size_t, i>;
    template <std::size_t i> constexpr SizeT<i> size_t{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
