/*!
@file
Defines `boost::hana::operators`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_OPERATORS_HPP
#define BOOST_HANA_CORE_OPERATORS_HPP

namespace boost { namespace hana {
    //! @namespace
    //! @todo document me:
    //! - this contains all the custom operators.
    //! - each operator is only enabled when explicitly asked for.
    //! - Each operator is also brought into hana's namespace via `using operators::xx`
    namespace operators {
        //! @ingroup group-core
        //! Allows custom Hana operators to be used with a type.
        //!
        //! Specifically, by inheriting from `enable_adl` or by using it as a
        //! template parameter, one makes the custom operators defined in the
        //! `operators` namespace accessible via argument-dependent lookup for
        //! that type. However, this does not change the fact that only the
        //! operators that were explicitly enabled with `enable_operators`
        //! will be usable.
        //!
        //! @todo
        //! We could use some reformulation here.
        //!
        //! ### Example
        //! @include example/core/enable_adl.cpp
        struct enable_adl { };

        //! @ingroup group-core
        //! @todo
        //! document me:
        //! makes the derived class ADL-findable for operators _and_ enable
        //! the operators for the given type classes at the same time. This
        //! only works when the data type is the same as the C++ type. this
        //! must be inherited; using this as a template parameter won't
        //! enable the operators (it will make them ADL-findable, but w/e).
        template <typename ...Typeclasses>
        struct enable : enable_adl {
            struct hana_enabled_operators : Typeclasses... { };
        };
    }

    namespace core_detail {
        template <typename Datatype, typename = void*>
        struct default_enabled_operators { };

        template <typename Datatype>
        struct default_enabled_operators<Datatype, decltype((void*)(
            (typename Datatype::hana_enabled_operators*)0
        ))>
            : Datatype::hana_enabled_operators
        { };

        template <typename Typeclass>
        constexpr bool enable_operators_impl(Typeclass*) { return true; }

        template <typename Typeclass>
        constexpr bool enable_operators_impl(...) { return false; }
    }

    //! @ingroup group-core
    //! @todo document me:
    //! you specialize this struct with your data type and inherit from the
    //! type classes for which you want the operators to be enabled. you still
    //! need to make the operators accessible, but for that you can use
    //! `enable_adl` or another trick if you wish.
    //!
    //! @todo add support for when<>
    template <typename Datatype>
    struct enabled_operators
        : core_detail::default_enabled_operators<Datatype>
    { };

    template <typename Typeclass, typename Datatype>
    struct enable_operators {
        static constexpr bool value = core_detail::enable_operators_impl<Typeclass>(
            (enabled_operators<Datatype>*)0
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_OPERATORS_HPP
