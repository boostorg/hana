/*!
@file
Defines the instance of `boost::hana::Logical` for `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_LOGICAL_HPP
#define BOOST_HANA_INTEGRAL_LOGICAL_HPP

#include <boost/hana/integral/integral.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    namespace integral_detail {
        template <bool b>
        struct eval_if_impl;

        template <>
        struct eval_if_impl<true> {
            template <typename T, typename E>
            static constexpr auto apply(T t, E)
            { return t([](auto x) { return x; }); }
        };

        template <>
        struct eval_if_impl<false> {
            template <typename T, typename E>
            static constexpr auto apply(T, E e)
            { return e([](auto x) { return x; }); }
        };
    }

    //! Instance of `Logical` for `Integral`s.
    //!
    //! An `Integral` is true-valued if and only if its integral value is
    //! true-valued, i.e. if
    //! @code
    //!     static_cast<bool>(n()) == true
    //! @endcode
    template <>
    struct Logical::instance<Integral> : Logical::mcd {
        template <typename Cond, typename Then, typename Else>
        static constexpr auto eval_if_impl(Cond c, Then t, Else e) {
            return integral_detail::eval_if_impl<
                static_cast<bool>(c())
            >::apply(t, e);
        }

        template <typename Cond>
        static constexpr auto not_impl(Cond c)
        { return integral<decltype(!c()), !c()>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_LOGICAL_HPP
