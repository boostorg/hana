/*!
@file
Defines the `BOOST_HANA_DEFINE_RECORD` and the
`BOOST_HANA_DEFINE_RECORD_INTRUSIVE` macros.

@note
This header carries a dependency on the Boost.Preprocessor library. For this
reason, it is not included by default by the master header of the library.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_MACROS_HPP
#define BOOST_HANA_RECORD_MACROS_HPP

#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/record.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/config/config.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/pop_back.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#if !BOOST_PP_VARIADICS && !defined(BOOST_HANA_DOXYGEN_INVOKED)
#   error "BOOST_PP_VARIADICS must be defined in order to use the functionality provided by this header"
#endif

#define BOOST_HANA_PP_STRINGIZE_PRIMITIVE(token) #token

#define BOOST_HANA_PP_STRINGIZE(token) \
    BOOST_HANA_PP_STRINGIZE_PRIMITIVE(token)

#define BOOST_HANA_PP_RECORD_MEMBER_TYPE(MEMBER) \
    BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_TUPLE_POP_BACK(MEMBER))

#define BOOST_HANA_PP_RECORD_MEMBER_NAME(MEMBER) \
    BOOST_PP_TUPLE_ELEM(BOOST_PP_DEC(BOOST_PP_TUPLE_SIZE(MEMBER)), MEMBER)

#define BOOST_HANA_PP_RECORD_TUPLE_MEMBER_IMPL(_, __, MEMBER)               \
    ::boost::hana::pair(                                                    \
        BOOST_HANA_STRING(BOOST_HANA_PP_STRINGIZE(                          \
            BOOST_HANA_PP_RECORD_MEMBER_NAME(MEMBER)                        \
        )),                                                                 \
        [](auto&& x) -> decltype(auto) {                                    \
            return ::boost::hana::id(                                       \
                ::boost::hana::detail::std::forward<decltype(x)>(x).        \
                    BOOST_HANA_PP_RECORD_MEMBER_NAME(MEMBER));              \
        }                                                                   \
    )                                                                       \
/**/

#define BOOST_HANA_PP_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)                  \
    static BOOST_HANA_CONSTEXPR_LAMBDA decltype(auto) apply() {             \
        return ::boost::hana::tuple(                                        \
            BOOST_PP_SEQ_ENUM(                                              \
                BOOST_PP_SEQ_TRANSFORM(                                     \
                    BOOST_HANA_PP_RECORD_TUPLE_MEMBER_IMPL, ~, MEMBERS      \
                )                                                           \
            )                                                               \
        );                                                                  \
    }                                                                       \
/**/

//! Defines a model of `Record` with the given members.
//! @relates boost::hana::Record
//!
//! Specifically, use this macro in the public section of a user-defined type
//! `T` to define a model of the `Record` type class for `T`. Note that
//! this only works if the data type of `T` is `T` itself.
//!
//!
//! Example
//! -------
//! @snippet example/record.macros.cpp intrusive
#define BOOST_HANA_DEFINE_RECORD_INTRUSIVE(...)                             \
    BOOST_HANA_PP_DEFINE_RECORD_INTRUSIVE_IMPL(                             \
        BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__),                             \
        BOOST_PP_SEQ_TAIL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))            \
    )                                                                       \
/**/

#define BOOST_HANA_PP_RECORD_DECLARE_MEMBER_IMPL(_, __, MEMBER)             \
    BOOST_HANA_PP_RECORD_MEMBER_TYPE(MEMBER)                                \
        BOOST_HANA_PP_RECORD_MEMBER_NAME(MEMBER);                           \
/**/

#define BOOST_HANA_PP_DEFINE_RECORD_INTRUSIVE_IMPL(DATATYPE, MEMBERS)       \
    BOOST_PP_SEQ_FOR_EACH(BOOST_HANA_PP_RECORD_DECLARE_MEMBER_IMPL, ~, MEMBERS)\
                                                                            \
    struct hana { struct members_impl {                                     \
        BOOST_HANA_PP_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)                  \
    }; }                                                                    \
/**/

//! Defines a model of `Record` with the given members.
//! @relates boost::hana::Record
//!
//! Specifically, use this macro at __global scope__ to define a model of
//! the `Record` concept for a given data type.
//!
//!
//! Example
//! -------
//! @snippet example/record.macros.cpp adhoc
#define BOOST_HANA_DEFINE_RECORD(...)                                       \
    BOOST_HANA_PP_DEFINE_RECORD_IMPL(                                       \
        BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__),                             \
        BOOST_PP_SEQ_TAIL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))            \
    )                                                                       \
/**/

#define BOOST_HANA_PP_DEFINE_RECORD_IMPL(DATATYPE, MEMBERS)                 \
    namespace boost { namespace hana {                                      \
        template <>                                                         \
        struct models< ::boost::hana::Record(DATATYPE)>                     \
            : ::boost::hana::detail::std::true_type                         \
        { };                                                                \
                                                                            \
        template <>                                                         \
        struct members_impl<DATATYPE> {                                     \
            BOOST_HANA_PP_RECORD_DEFINE_INSTANCE_IMPL(MEMBERS)              \
        };                                                                  \
    }}                                                                      \
/**/

#endif // !BOOST_HANA_RECORD_MACROS_HPP
