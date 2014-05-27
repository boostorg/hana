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

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>


namespace boost { namespace hana {
    struct _Integral;

    //! @ingroup datatypes
    template <typename T, T t>
    struct Integral {
        constexpr operator T() const { return t; }
        using hana_datatype = _Integral;
    };

    template <>
    struct Comparable<_Integral, _Integral> : defaults<Comparable> {
        template <typename T, T t, typename U, U u>
        static constexpr Integral<bool, t == u>
        equal_impl(Integral<T, t>, Integral<U, u>)
        { return {}; }

        template <typename T, T t, typename U, U u>
        static constexpr Integral<bool, t != u>
        not_equal_impl(Integral<T, t>, Integral<U, u>)
        { return {}; }
    };

#define BOOST_HANA_INTEGRAL_BINARY_OP(op)                                   \
    template <typename U, U u, typename V, V v>                             \
    constexpr Integral<decltype(u op v), (u op v)>                          \
    operator op(Integral<U, u>, Integral<V, v>)                             \
    { return {}; }                                                          \
/**/
#define BOOST_HANA_INTEGRAL_UNARY_OP(op)                                    \
    template <typename U, U u>                                              \
    constexpr Integral<decltype(op u), (op u)>                              \
    operator op(Integral<U, u>)                                             \
    { return {}; }                                                          \
/**/
    // Arithmetic
    BOOST_HANA_INTEGRAL_UNARY_OP(+)
    BOOST_HANA_INTEGRAL_UNARY_OP(-)
    BOOST_HANA_INTEGRAL_BINARY_OP(+)
    BOOST_HANA_INTEGRAL_BINARY_OP(-)
    BOOST_HANA_INTEGRAL_BINARY_OP(*)
    BOOST_HANA_INTEGRAL_BINARY_OP(/)
    BOOST_HANA_INTEGRAL_BINARY_OP(%)
    BOOST_HANA_INTEGRAL_UNARY_OP(~)
    BOOST_HANA_INTEGRAL_BINARY_OP(&)
    BOOST_HANA_INTEGRAL_BINARY_OP(|)
    BOOST_HANA_INTEGRAL_BINARY_OP(^)
    BOOST_HANA_INTEGRAL_BINARY_OP(<<)
    BOOST_HANA_INTEGRAL_BINARY_OP(>>)

    // Comparison
    BOOST_HANA_INTEGRAL_BINARY_OP(==)
    BOOST_HANA_INTEGRAL_BINARY_OP(!=)
    BOOST_HANA_INTEGRAL_BINARY_OP(<)
    BOOST_HANA_INTEGRAL_BINARY_OP(<=)
    BOOST_HANA_INTEGRAL_BINARY_OP(>)
    BOOST_HANA_INTEGRAL_BINARY_OP(>=)

    // Logical
    BOOST_HANA_INTEGRAL_BINARY_OP(||)
    BOOST_HANA_INTEGRAL_BINARY_OP(&&)
    BOOST_HANA_INTEGRAL_UNARY_OP(!)
#undef BOOST_HANA_INTEGRAL_UNARY_OP
#undef BOOST_HANA_INTEGRAL_BINARY_OP

    template <int i> using Int = Integral<int, i>;
    template <int i> constexpr Int<i> int_{};

    template <bool b> using Bool = Integral<bool, b>;
    template <bool b> constexpr Bool<b> bool_{};
    constexpr auto true_ = bool_<true>;
    constexpr auto false_ = bool_<false>;

    namespace detail { using std_size_t = decltype(sizeof(int)); }
    template <detail::std_size_t n>
    using SizeT = Integral<detail::std_size_t, n>;
    template <detail::std_size_t n>
    constexpr SizeT<n> size_t{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
