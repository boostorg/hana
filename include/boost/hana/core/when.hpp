/*!
@file
Defines `boost::hana::when` and `boost::hana::when_valid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_WHEN_HPP
#define BOOST_HANA_CORE_WHEN_HPP

namespace boost { namespace hana {
    //! @ingroup group-core
    //! Enable a partial specialization only if a boolean condition is true.
    //!
    //! > #### Rationale for using `when` instead of `std::enable_if`
    //! > `when` is used to control the priority of partial specializations
    //! > in a finer grained manner than what can be achieved with the usual
    //! > `typename Enable = void` and `std::enable_if` pattern. For example,
    //! > a partially specialized tag-dispatched method will have a higher
    //! > priority than an equivalent specialization that uses `when`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/when.cpp when
    template <bool condition>
    struct when;

    namespace core_detail {
        template <typename ...>
        static constexpr bool valid = true;
    }

    //! @ingroup group-core
    //! Variant of `when` allowing specializations to be enabled only if an
    //! expression is well-formed.
    //!
    //! `when_valid<...>` is always equivalent to `when<true>`. However, when
    //! used inside a partial specialization, SFINAE will cause the partial
    //! specialization to be ignored when the expression is ill-formed.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/core/when.cpp when_valid
    template <typename ...T>
    using when_valid = when<core_detail::valid<T...>>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_WHEN_HPP
