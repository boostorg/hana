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
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    struct _Type;

    namespace operators {
        //! @ingroup datatypes
        template <typename T>
        struct Type {
            using hana_datatype = _Type;
            using type = T;
        };
    }
    using operators::Type;

    template <typename T>
    constexpr Type<T> type{};

    //! @ingroup datatypes
    template <template <typename ...> class f>
    struct Template { };

    template <template <typename ...> class f>
    constexpr Template<f> template_{};

    /*!
     * Inverse of `unit`.
     *
     * This is a partial function. It is only defined for default
     * constructible `T`s.
     */
    template <typename T>
    constexpr T untype(Type<T>)
    { return {}; }

    template <>
    struct Comparable<_Type, _Type> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(Type<T>, Type<U>)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(Type<T>, Type<T>)
        { return true_; }
    };


    template <>
    struct Functor<_Type> : defaults<Functor> {
        template <template <typename ...> class F, typename T>
        static constexpr Type<F<T>> fmap_impl(Template<F>, Type<T>)
        { return {}; }
    };

    template <>
    struct Monad<_Type> : defaults<Monad> {
        template <typename T>
        static constexpr Type<T> unit_impl(T)
        { return {}; }

        template <typename T>
        static constexpr Type<T> join_impl(Type<Type<T>>)
        { return {}; }
    };

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
