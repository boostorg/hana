/*!
@file
Forward declares `boost::hana::Bool`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_BOOL_HPP
#define BOOST_HANA_FWD_BOOL_HPP

#include <boost/hana/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time boolean value.
    //!
    //! Modeled concepts
    //! ----------------
    //! - `Comparable` (operators provided)\n
    //! @todo
    //!
    //! - `Constant`\n
    //! @todo
    //!
    //! - `Logical` (operators provided)\n
    //! @todo
    struct Bool { using value_type = bool; };

    //! Creates a `Bool` with the given truth-value.
    //! @relates Bool
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <bool v>
    constexpr unspecified-type bool_{};
#else
    template <bool v>
    struct _bool : operators::enable_adl {
        using type = _bool;
        using value_type = bool;
        static constexpr value_type value = v;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }

        struct hana { using datatype = Bool; };
    };

    template <bool b>
    constexpr _bool<b> bool_{};
#endif

    //! Equivalent to `bool_<true>`.
    //! @relates Bool
    constexpr auto true_ = bool_<true>;

    //! Equivalent to `bool_<false>`.
    //! @relates Bool
    constexpr auto false_ = bool_<false>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_BOOL_HPP
