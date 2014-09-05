/*!
@file
Defines custom operators for `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_OPERATORS_HPP
#define BOOST_HANA_ITERABLE_OPERATORS_HPP

#include <boost/hana/iterable/iterable.hpp>

#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana { namespace operators {
    //! Inherit this to get an `operator[]` equivalent to `at`.
    //! @relates boost::hana::Iterable
    //!
    //! @todo Find a proper way to provide member operators.
    template <typename Derived>
    struct Iterable_ops {
        template <typename I>
        constexpr decltype(auto) operator[](I&& i) const& {
            return at(
                detail::std::forward<I>(i),
                static_cast<Derived const&>(*this)
            );
        }

        template <typename I>
        constexpr decltype(auto) operator[](I&& i) & {
            return at(
                detail::std::forward<I>(i),
                static_cast<Derived&>(*this)
            );
        }

        template <typename I>
        constexpr decltype(auto) operator[](I&& i) const&& {
            return at(
                detail::std::forward<I>(i),
                static_cast<Derived const&&>(*this)
            );
        }

        template <typename I>
        constexpr decltype(auto) operator[](I&& i) && {
            return at(
                detail::std::forward<I>(i),
                static_cast<Derived&&>(*this)
            );
        }
    };
}}} // end namespace boost::hana::operators

#endif // !BOOST_HANA_ITERABLE_OPERATORS_HPP
