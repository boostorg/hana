/*!
@file
Defines `boost::hana::convert` and `boost::hana::to`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_CONVERT_HPP
#define BOOST_HANA_CORE_CONVERT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //!
    //!
    //! embeddings:
    //! It is often useful to embed a smaller algebraic structure into a bigger one, for
    //! example by taking an integer to an equivalent floating point number.
    //! For the conversion to be well-behaved, it is necessary that this coversion
    //! is lossless
    //!
    //!    However, note that the semantics associated with `to<C>` are
    //!    not checked and the only requirement for compilation to succeed
    //!    is that `to<C>` be defined. That being said, you should ensure
    //!    that the semantics are respected unless you want to shoot yourself
    //!    in the foot.
    //!
    //! @todo Document
    struct embedding { };

    //! @ingroup group-core
    //!
    //!
    //! @todo Document
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Conversion>
    struct is_embedding : std::is_base_of<embedding, Conversion> { };
#else
    template <typename Conversion, typename = void>
    struct is_embedding : detail::std::false_type { };

    template <typename Conversion>
    struct is_embedding<Conversion, decltype((void)
        static_cast<embedding>(*(Conversion*)0)
    )> : detail::std::true_type { };
#endif

    //! @ingroup group-core
    //! Implements conversions between data types.
    //!
    //! @todo UPDATE THE DOCUMENTATION
    //! ----------------------------------------------------------------------
    //!
    //! To specify a conversion between two data types, one must specialize
    //! `convert` in the `boost::hana` namespace for the corresponding data
    //! types. `when` can be used to perform flexible specialization.
    //!
    //! By default, `convert` has the following behavior:
    //! If the `To` and `From` data types are the same, the object is
    //! forwarded as-is. Otherwise, if the type of the converted-from object
    //! -- its actual type, not its hana data type -- is convertible to the
    //! `To` data type with `static_cast`, that conversion is used. Otherwise,
    //! calling `convert<To, From>::%apply` triggers a static assertion.
    //!
    //! @note
    //! `convert` is only used to provide the conversions; to actually
    //! perform conversions, use `to`.
    //!
    //! @todo
    //! Use more clever dispatching for this so it is possible to ask whether
    //! a conversion exists, and then use this to implement `common`.
    //!
    //! ### Example
    //! @include example/core/convert.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename To, typename From, optional when-based enabler>
    struct convert { };
#else
    template <typename To, typename From, typename = void>
    struct convert : convert<To, From, when<true>> { };

    template <typename To, typename From, bool condition>
    struct convert<To, From, when<condition>> {
        static_assert(wrong<convert<To, From>>{},
        "no conversion is available between the provided data types");
    };

    template <typename To, typename From>
    struct convert<To, From,
        when_valid<decltype(static_cast<To>(detail::std::declval<From>()))>>
    {
        template <typename X>
        static constexpr To apply(X&& x)
        { return static_cast<To>(detail::std::forward<X>(x)); }
    };

    template <typename To>
    struct convert<To, To> : embedding {
        template <typename X>
        static constexpr X apply(X&& x)
        { return detail::std::forward<X>(x); }
    };

#define BOOST_HANA_DEFINE_EMBEDDING_IMPL(TO, FROM)                          \
    template <>                                                             \
    struct convert<TO, FROM> : embedding                                    \
    { static constexpr TO apply(FROM x) { return x; } }                     \
/**/
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long double, double);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long double, float);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(double     , float);

    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long int, long int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long long int, int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(long int     , int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(int          , short int);

    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long int, unsigned long int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long long int, unsigned int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned long int     , unsigned int);
    BOOST_HANA_DEFINE_EMBEDDING_IMPL(unsigned int          , unsigned short int);
#undef BOOST_HANA_DEFINE_EMBEDDING_IMPL
#endif

    //! @ingroup group-core
    //! Create an object of a data type from an object of another data type.
    //!
    //! @todo UPDATE THE DOCUMENTATION
    //! ----------------------------------------------------------------------
    //!
    //! See `convert` to specify _how_ to convert from a data type to another.
    //!
    //!
    //! @tparam To
    //! The data type to which `x` should be converted.
    //!
    //! @param x
    //! The object to convert to the given data type.
    //!
    //!
    //! ### Example
    //! @snippet example/core/to.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename To>
    constexpr auto to = [](auto&& x) -> decltype(auto) {
        return convert<To, datatype_t<decltype(x)>>::apply(
            std::forward<decltype(x)>(x)
        );
    };
#else
    template <typename To>
    struct _to {
        template <typename X>
        constexpr auto operator()(X&& x) const -> decltype(
            convert<To, typename datatype<X>::type>::apply(
                                                detail::std::forward<X>(x))
        ) {
            return convert<To, typename datatype<X>::type>::apply(
                                                detail::std::forward<X>(x));
        }
    };

    template <typename To>
    constexpr _to<To> to{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_CONVERT_HPP
