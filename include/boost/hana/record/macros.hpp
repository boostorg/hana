/*!
@file
Defines the `BOOST_HANA_DEFINE_RECORD` and the
`BOOST_HANA_DEFINE_RECORD_INTRUSIVE` macros.

@note
This header carries a dependency on the Boost.Preprocessor library. For this
reason, it is not included by default by boost/hana/record.hpp.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_MACROS_HPP
#define BOOST_HANA_RECORD_MACROS_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/record/mcd.hpp>
#include <boost/hana/tuple.hpp>

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/transform.hpp>


#define BOOST_HANA_RECORD_TUPLE_MEMBER_IMPL(_, __, MEMBER)                  \
    ::boost::hana::pair(                                                    \
        BOOST_PP_SEQ_ELEM(0, MEMBER),                                       \
        [](auto&& x) -> decltype(auto) {                                    \
        return ::boost::hana::detail::std::forward<decltype(x)>(x).         \
            BOOST_PP_SEQ_ELEM(2, MEMBER);                                   \
    })                                                                      \
/**/

#define BOOST_HANA_RECORD_DECLARE_MEMBER_IMPL(_, __, MEMBER) \
    BOOST_PP_SEQ_ELEM(1, MEMBER) BOOST_PP_SEQ_ELEM(2, MEMBER);

#define BOOST_HANA_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)                     \
    static BOOST_HANA_CONSTEXPR_LAMBDA auto members_impl() {                \
        return ::boost::hana::tuple(                                        \
            BOOST_PP_SEQ_ENUM(                                              \
                BOOST_PP_SEQ_TRANSFORM(                                     \
                    BOOST_HANA_RECORD_TUPLE_MEMBER_IMPL, ~, MEMBERS         \
                )                                                           \
            )                                                               \
        );                                                                  \
    }                                                                       \
/**/

//! Defines an instance of `Record` with the given members.
//! @relates bost::hana::Record
//!
//! Specifically, use this macro in the public section of a user-defined type
//! `T` to define an instance of the `Record` type class for `T`. Note that
//! this only works if the data type of `T` is `T` itself.
//!
//! ### Example
//! @snippet example/record/macros.cpp intrusive
#define BOOST_HANA_DEFINE_RECORD_INTRUSIVE(DATATYPE, MEMBERS)               \
    BOOST_PP_SEQ_FOR_EACH(BOOST_HANA_RECORD_DECLARE_MEMBER_IMPL, ~, MEMBERS)\
                                                                            \
    struct hana_Record : ::boost::hana::Record::mcd {                       \
        BOOST_HANA_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)                     \
    }                                                                       \
/**/

//! Defines an instance of `Record` with the given members.
//! @relates bost::hana::Record
//!
//! Specifically, use this macro at __global scope__ to define an instance of
//! the `Record` type class for a given data type.
//!
//! ### Example
//! @snippet example/record/macros.cpp adhoc
#define BOOST_HANA_DEFINE_RECORD(DATATYPE, MEMBERS)                         \
    namespace boost { namespace hana {                                      \
        template <>                                                         \
        struct Record::instance<DATATYPE>                                   \
            : Record::mcd                                                   \
        {                                                                   \
            BOOST_HANA_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)                 \
        };                                                                  \
    }}                                                                      \
/**/

#endif // !BOOST_HANA_RECORD_MACROS_HPP
