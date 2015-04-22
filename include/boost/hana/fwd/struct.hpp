/*!
@file
Forward declares `boost::hana::Struct`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_STRUCT_HPP
#define BOOST_HANA_FWD_STRUCT_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Struct` concept represents `struct`-like user-defined types.
    //!
    //! The `Struct` concept allows restricted compile-time reflection over
    //! user-defined types. In particular, it allows accessing the names of
    //! the members of a user-defined type, and also the value of those
    //! members. `Struct`s can also be folded, searched and converted to
    //! some types of containers, where more advanced transformations can
    //! be performed.
    //!
    //! While all types can _in theory_ be made `Struct`s, only a subset of
    //! them are actually interesting to see as such. More precisely, it is
    //! only interesting to make a type a `Struct` when it is conceptually
    //! a C++ `struct`, i.e. a mostly dumb aggregate of named data. The way
    //! this data is accessed is mostly unimportant to the `Struct` concept;
    //! it could be through getters and setters, through public members,
    //! through non-member functions or it could even be generated on-the-fly.
    //! The important part, which is made precise below, is that those accessor
    //! methods should be move-independent.
    //!
    //! Another way to see a `Struct` is as a map where the keys are the names
    //! of the members and the values are the values of those members. However,
    //! there are subtle differences like the fact that one can't add a member
    //! to a `Struct`, and also that the order of the members inside a `Struct`
    //! plays a role in determining the equality of `Struct`s, which is not
    //! the case for maps.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! __Define the `accessors` method__\n
    //! A model of `Struct` is created by specifying a sequence of key/value
    //! pairs with the `accessors` function. The first element of a pair in
    //! this sequence represents the "name" of a member of the `Struct`, while
    //! the second element is a function which retrieves this member from an
    //! object. The "names" do not have to be in any special form; they just
    //! have to be compile-time `Comparable`. For example, it is common to
    //! provide "names" that are `String`s representing the actual names of
    //! the members, but one could provide `IntegralConstant`s just as well.
    //! The values must be functions which, when given an object, retrieve the
    //! appropriate member from it. It is not important that the functions
    //! actually retrieve a member (e.g. `x.member`); what is important is
    //! that they are move-independent, a notion which is defined below.
    //!
    //! There are several ways of providing the `accessors` method, some of
    //! which are more flexible and others which are more convenient. First,
    //! one can define it through tag-dispatching, as usual.
    //! @snippet example/struct.mcd.tag_dispatching.cpp main
    //!
    //! Secondly, it is possible to provide a nested `hana::accessors_impl`
    //! type, which should be equivalent to a specialization of
    //! `accessors_impl` for tag-dispatching. However, for a type `S`, this
    //! technique only works when the data type of `S` is `S` itself, which
    //! is the case unless you explicitly asked for something else.
    //! @snippet example/struct.mcd.nested.cpp main
    //!
    //! Finally, the most convenient (but least flexible) option is to use
    //! the `BOOST_HANA_DEFINE_STRUCT` or the `BOOST_HANA_ADAPT_STRUCT` macro
    //! to provide a model of the `Struct` concept with minimal syntactic
    //! overhead. See the documentation of these macros for details on how
    //! to use them.
    //!
    //!
    //! @anchor move-independence
    //! Move-independence
    //! -----------------
    //! The notion of move-independence presented here defines rigorously
    //! when it is legitimate to "double-move" from an object.
    //!
    //! A collection of functions `f1, ..., fn` sharing the same domain is
    //! said to be _move-independent_ if for every fresh (not moved-from)
    //! object `x` in the domain, any permutation of the following statements
    //! is valid and leaves the `zk` objects in a fresh (not moved-from) state:
    //! @code
    //!     auto z1 = f1(std::move(x));
    //!     ...
    //!     auto zn = fn(std::move(x));
    //! @endcode
    //!
    //! @note
    //! In the special case where some functions return objects that can't be
    //! bound to with `auto zk =` (like `void` or a non-movable, non-copyable
    //! type), just pretend the return value is ignored.
    //!
    //! Intuitively, this ensures that we can treat `f1, ..., fn` as
    //! "accessors" that decompose `x` into independent subobjects, and
    //! that do so without moving from `x` more than that subobject. This
    //! is important because it allows us to optimally decompose `Struct`s
    //! into their subparts inside the library.
    //!
    //!
    //! Laws
    //! ----
    //! For any `Struct` `S`, the accessors in the `accessors<S>()` sequence
    //! must be move-independent, as defined above.
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Comparable` (model provided)\n
    //! `Struct`s are required to be `Comparable`. Specifically, two `Struct`s
    //! of the same data type `S` must be equal if and only if all of their
    //! members are equal. By default, a model of `Comparable` doing just that
    //! is provided for models of `Struct`. In particular, note that the
    //! comparison of the members is made in the same order as they appear in
    //! the `hana::members` sequence.
    //! @snippet example/struct.cpp Comparable
    //!
    //! 2. `Foldable` (model provided)\n
    //! A `Struct` can be folded by considering it as a list of pairs each
    //! containing the name of a member and the value associated to that
    //! member, in the same order as they appear in the `hana::members`
    //! sequence. By default, a model of `Foldable` doing just that is
    //! provided for models of the `Struct` concept.
    //! @snippet example/struct.cpp Foldable
    //!
    //! Being a model of `Foldable` makes it possible to turn a `Struct`
    //! into basically any `Sequence`, but also into a `Map` by simply
    //! using the `to<...>` function!
    //! @snippet example/struct.cpp to<Map>
    //!
    //! 3. `Searchable` (model provided)\n
    //! A `Struct` can be searched by considering it as a `Map` where the keys
    //! are the names of the members of the `Struct`, and the values are the
    //! members associated to those names. By default, a model of `Searchable`
    //! is provided for any model of the `Struct` concept.
    //! @snippet example/struct.cpp Searchable
    struct Struct { };

    //! Returns a `Sequence` of `Pair`s representing the accessors of the
    //! data structure.
    //! @relates Struct
    //!
    //! Given a `Struct` `S`, `accessors<S>()` is a `Sequence` of `Product`s
    //! where the first element of each pair is the "name" of a member of
    //! the `Struct`, and the second element of each pair is a function that
    //! can be used to access that member when given an object of the proper
    //! data type. As described in the global documentation for `Struct`, the
    //! accessor functions in this sequence must be move-independent.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/struct.cpp accessors
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename S>
    constexpr auto accessors = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct accessors_impl;

    template <typename S>
    struct _accessors {
    #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Struct, S>{},
        "hana::accessors<S> requires S to be a Struct");
    #endif

        constexpr decltype(auto) operator()() const
        { return accessors_impl<S>::apply(); }
    };

    template <typename S>
    constexpr _accessors<S> accessors{};
#endif

    //! Returns a `Sequence` containing the members of a `Struct`.
    //! @relates Struct
    //!
    //! Given a `Struct` object, `members` returns a `Sequence` containing
    //! all the members of the `Struct`, in the same order as their respective
    //! accessor appears in the `accessors` sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/struct.cpp members
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto members = [](auto&& struct_) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct members_impl;

    struct _members {
        template <typename Object>
        constexpr decltype(auto) operator()(Object&& object) const {
            using S = typename datatype<Object>::type;

            #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
                static_assert(_models<Struct, S>{},
                "hana::members(object) requires object to be a Struct");
            #endif

            return members_impl<S>::apply(static_cast<Object&&>(object));
        }
    };

    constexpr _members members{};
#endif

    // Note:
    // For the two macros below, their weird definition as variables seems to
    // exploit a glitch in Doxygen, which makes the macros appear in the
    // related objects of Struct (as we want them to).

    //! Defines members of a structure, while at the same time
    //! modeling `Struct`.
    //! @relates Struct
    //!
    //! Using this macro in the body of a user-defined type will define the
    //! given members inside that type, and will also provide a model of the
    //! `Struct` concept for that user-defined type. This macro is often the
    //! easiest way to define a model of the `Struct` concept.
    //!
    //! @remark
    //! This macro is defined in the `boost/hana/struct_macros.hpp` header.
    //! It is not included in `boost/hana/struct.hpp` because it depends on
    //! the Boost.Preprocessor library. It is not included by the master
    //! header of the library for the same reason.
    //!
    //! @note
    //! This macro only works if the data type (generalized type) of the
    //! user-defined type `T` is `T` itself. This is the case unless you
    //! specifically asked for something different; see `datatype`'s
    //! documentation.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/struct.mcd.define_macro.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    auto BOOST_HANA_DEFINE_STRUCT(...) = ;
    #define BOOST_HANA_DEFINE_STRUCT(Name, ...) see documentation
#else
    // defined in boost/hana/struct_macros.hpp
#endif

    //! Defines a model of `Struct` with the given members.
    //! @relates Struct
    //!
    //! Using this macro at _global scope_ will define a model of the `Struct`
    //! concept for the given type. This can be used to easily adapt existing
    //! user-defined types in a ad-hoc manner.
    //!
    //! @remark
    //! This macro is defined in the `boost/hana/struct_macros.hpp` header.
    //! It is not included in `boost/hana/struct.hpp` because it depends on
    //! the Boost.Preprocessor library. It is not included by the master
    //! header of the library for the same reason.
    //!
    //! @note
    //! This macro only works if the data type (generalized type) of the
    //! user-defined type `T` is `T` itself. This is the case unless you
    //! specifically asked for something different; see `datatype`'s
    //! documentation.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/struct.mcd.adapt_macro.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    auto BOOST_HANA_ADAPT_STRUCT(...) = ;
    #define BOOST_HANA_ADAPT_STRUCT(Name, ...) see documentation
#else
    // defined in boost/hana/struct_macros.hpp
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_STRUCT_HPP
