/*!
 * @file
 * Defines `boost::hana::Type`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    struct _Type;

    //! @ingroup datatypes
    template <typename T>
    struct Type {
        using hana_datatype = _Type;
        using type = T;
    };

    template <typename T>
    constexpr Type<T> type{};

    template <>
    struct Comparable<_Type, _Type> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(Type<T>, Type<U>)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(Type<T>, Type<T>)
        { return true_; }
    };

    template <typename T, typename U>
    constexpr auto operator==(Type<T> t, Type<U> u)
    { return equal(t, u); }

    template <typename T, typename U>
    constexpr auto operator!=(Type<T> t, Type<U> u)
    { return not_equal(t, u); }


    namespace type_detail {
        template <template <typename ...> class f>
        struct Lift {
            template <typename ...Args>
            constexpr auto operator()(Type<Args>...) const
            { return type<f<Args...>>; }
        };
    }

    template <template <typename ...> class f>
    constexpr auto lift = type_detail::Lift<f>{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
