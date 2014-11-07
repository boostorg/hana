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
    //! @internal
    //! ### Rationale for using `when` instead of `std::enable_if`
    //! Using `when` is necessary for two reasons. First, a non-type template
    //! argument may not depend on a template parameter of a partial
    //! specialization, so we need to wrap the `bool` condition into a
    //! type. Second, `when` is used to control the priority of partial
    //! specializations in a finer grained manner than what can be achieved
    //! with the usual `typename Enable = void` and `std::enable_if` pattern.
    //! @endinternal
    //!
    //! ### Example
    //! @include example/core/when.cpp
    template <bool condition>
    struct when;

    //! @ingroup group-core
    //! Variant of `when` allowing specializations to be enabled only if an
    //! expression is well-formed.
    //!
    //! `when_valid<...>` is always equivalent to `when<true>`. However, when
    //! used inside a partial specialization, SFINAE will cause the partial
    //! specialization to be ignored when the expression is ill-formed.
    //!
    //! ### Example
    //! @include example/core/when_valid.cpp
    template <typename ...>
    using when_valid = when<true>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_WHEN_HPP
