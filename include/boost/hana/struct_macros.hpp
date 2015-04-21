/*!
@file
Defines the `BOOST_HANA_DEFINE_STRUCT` and the `BOOST_HANA_ADAPT_STRUCT` macros.

@note
This header carries a dependency on the Boost.Preprocessor library. For this
reason, it is not included by default by the master header of the library.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRUCT_MACROS_HPP
#define BOOST_HANA_STRUCT_MACROS_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/tuple.hpp>

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/config/config.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/pop_back.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>


#if !BOOST_PP_VARIADICS && !defined(BOOST_HANA_DOXYGEN_INVOKED)
#   error "BOOST_PP_VARIADICS must be defined in order to use the functionality provided by this header"
#endif

namespace boost { namespace hana { namespace detail {
    template <typename Memptr, Memptr ptr>
    struct member_ptr {
        template <typename T>
        constexpr decltype(auto) operator()(T&& t) const
        { return static_cast<T&&>(t).*ptr; }
    };
}}}

#define BOOST_HANA_PP_STRUCT_ACCESSOR_PAIR(TYPE, MEMBER_TYPE, MEMBER_NAME)  \
    ::boost::hana::make_pair(                                               \
        BOOST_HANA_STRING(BOOST_PP_STRINGIZE(MEMBER_NAME)),                 \
        ::boost::hana::detail::member_ptr<                                  \
            MEMBER_TYPE TYPE::*, &TYPE::MEMBER_NAME                         \
        >{}                                                                 \
    )                                                                       \
/**/

#define BOOST_HANA_PP_STRUCT_ACCESSOR_PAIR_(_, TYPE, MEMBER)                \
    BOOST_HANA_PP_STRUCT_ACCESSOR_PAIR(                                     \
        TYPE,                                                               \
        BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_TUPLE_POP_BACK(MEMBER)),           \
        BOOST_PP_TUPLE_ELEM(BOOST_PP_DEC(BOOST_PP_TUPLE_SIZE(MEMBER)), MEMBER)\
    )                                                                       \
/**/

#define BOOST_HANA_PP_STRUCT_ACCESSORS_APPLY(TYPE, MEMBERS)                 \
    static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {                       \
        return ::boost::hana::make_tuple(                                   \
            BOOST_PP_SEQ_ENUM(                                              \
                BOOST_PP_SEQ_TRANSFORM(                                     \
                    BOOST_HANA_PP_STRUCT_ACCESSOR_PAIR_, TYPE, MEMBERS      \
                )                                                           \
            )                                                               \
        );                                                                  \
    }                                                                       \
/**/

#define BOOST_HANA_PP_IGNORE(...) /* nothing */

//////////////////////////////////////////////////////////////////////////////
// BOOST_HANA_DEFINE_STRUCT
//////////////////////////////////////////////////////////////////////////////
#define BOOST_HANA_DEFINE_STRUCT(...)                                       \
    BOOST_HANA_PP_DEFINE_STRUCT_IMPL(                                       \
        BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__),                             \
        BOOST_PP_SEQ_TAIL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)),           \
        BOOST_PP_DEC(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))                   \
    )                                                                       \
/**/

#define BOOST_HANA_PP_STRUCT_DECLARE_MEMBER(_, __, MEMBER)                  \
    BOOST_PP_TUPLE_REM_CTOR(BOOST_PP_TUPLE_POP_BACK(MEMBER))                \
    BOOST_PP_TUPLE_ELEM(BOOST_PP_DEC(BOOST_PP_TUPLE_SIZE(MEMBER)), MEMBER); \
/**/

#define BOOST_HANA_PP_DEFINE_STRUCT_IMPL(TYPE, MEMBERS, HAS_MEMBERS)        \
    BOOST_PP_IF(HAS_MEMBERS,                                                \
        BOOST_PP_SEQ_FOR_EACH,                                              \
        BOOST_HANA_PP_IGNORE                                                \
    )(BOOST_HANA_PP_STRUCT_DECLARE_MEMBER, ~, MEMBERS)                      \
                                                                            \
    struct hana { struct accessors_impl {                                   \
        BOOST_PP_IF(HAS_MEMBERS,                                            \
            BOOST_HANA_PP_STRUCT_ACCESSORS_APPLY,                           \
            static constexpr auto apply()                                   \
            { return ::boost::hana::make_tuple(); }                         \
            BOOST_HANA_PP_IGNORE                                            \
        )(TYPE, MEMBERS)                                                    \
    }; }                                                                    \
/**/

//////////////////////////////////////////////////////////////////////////////
// BOOST_HANA_ADAPT_STRUCT
//////////////////////////////////////////////////////////////////////////////
#define BOOST_HANA_ADAPT_STRUCT(...)                                        \
    BOOST_HANA_PP_ADAPT_STRUCT_IMPL(                                        \
        BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__),                             \
        BOOST_PP_SEQ_TAIL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)),           \
        BOOST_PP_DEC(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))                   \
    )                                                                       \
/**/

#define BOOST_HANA_PP_ADAPT_STRUCT_IMPL(TYPE, MEMBERS, HAS_MEMBERS)         \
    namespace boost { namespace hana {                                      \
        template <>                                                         \
        struct accessors_impl<TYPE> {                                       \
            BOOST_PP_IF(HAS_MEMBERS,                                        \
                BOOST_HANA_PP_STRUCT_ACCESSORS_APPLY,                       \
                static constexpr auto apply()                               \
                { return ::boost::hana::make_tuple(); }                     \
                BOOST_HANA_PP_IGNORE                                        \
            )(TYPE, MEMBERS)                                                \
        };                                                                  \
    }}                                                                      \
/**/

#endif // !BOOST_HANA_STRUCT_MACROS_HPP
