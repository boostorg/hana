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
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time boolean value.
    //!
    //! ### Instance of
    //! `Comparable`, `Constant` and `Logical`
    struct Bool { struct hana_enabled_operators : Comparable, Logical { }; };

    namespace bool_detail {
        template <bool v>
        struct bool_ : operators::enable_adl {
            using type = bool_;
            using value_type = bool;
            static constexpr value_type value = v;
            constexpr operator value_type() const noexcept { return value; }
            constexpr value_type operator()() const noexcept { return value; }

            struct hana { using datatype = Bool; };
        };
    }

    //! Creates a `Bool` with the given truth-value.
    //! @relates Bool
    template <bool b>
    constexpr bool_detail::bool_<b> bool_{};

    //! Equivalent to `bool_<true>`.
    //! @relates Bool
    constexpr auto true_ = bool_<true>;

    //! Equivalent to `bool_<false>`.
    //! @relates Bool
    constexpr auto false_ = bool_<false>;
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_BOOL_HPP
