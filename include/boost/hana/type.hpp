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
    //! @ingroup datatypes
    template <typename T>
    struct Type { using type = T; };

    template <typename T>
    constexpr Type<T> type{};

    template <typename T, typename U>
    struct Comparable<Type<T>, Type<U>> : defaults<Comparable> {
        static constexpr auto equal_impl(...) { return false_; }
        static constexpr auto not_equal_impl(...) { return true_; }
    };

    template <typename T>
    struct Comparable<Type<T>, Type<T>> : defaults<Comparable> {
        static constexpr auto equal_impl(...) { return true_; }
        static constexpr auto not_equal_impl(...) { return false_; }
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
