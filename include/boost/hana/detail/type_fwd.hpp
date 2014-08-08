/*!
@file
Internal header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_TYPE_FWD_HPP
#define BOOST_HANA_DETAIL_TYPE_FWD_HPP

#include <boost/hana/comparable/detail/comparable_fwd.hpp>
#include <boost/hana/core/datatype.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Object representing a C++ type.
    //!
    //! `Type`s are the key element making it possible to do everything that
    //! can be done with Boost.MPL also doable with Hana. A `Type` is an
    //! object of an unspecified type which represents (or wraps) a C++ type.
    //! However, since it is a regular C++ object, it can be stored in
    //! heterogeneous containers, passed as an argument to regular functions
    //! and so on. While representing C++ types as values might seem backward
    //! at first, it is in fact the very thing that makes it possible to unify
    //! type-level programming and value-level programming. In particular,
    //! this allows using the same syntax for type-level computations as used
    //! in everyday C++, and it even allows certain functions with very few
    //! requirements to be used both on types and on values, thus reducing
    //! code duplication. In comparison, previous metaprogramming libraries
    //! like [Boost.MPL][] or [MPL11][] used struct-based metafunctions that
    //! worked at the type level only, were less expressive due to the lack
    //! of anonymous functions and forced the programmer to use an awkward
    //! syntax.
    //!
    //! Of course, since each object of data type `Type` has an unspecified
    //! (and hence possibly different) C++ type, manipulating these guys
    //! requires being able to manipulate heterogeneous objects. Fortunately,
    //! this is exactly what this library was built for, so we're good.
    //!
    //! So far, we're only able to pass wrapped types around, which isn't
    //! very useful. For `Type`s to be of some use, they have to give us
    //! a way of getting that wrapped type out. Hence, for any `Type` `t`,
    //! `decltype(t)::type` is an alias to the type it wraps. In Boost.MPL
    //! parlance, `decltype(t)` is a nullary metafunction returning the C++
    //! type `t` represents.
    //!
    //! [Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
    //! [MPL11]: http://github.com/ldionne/mpl11
    //!
    //! ## Instance of
    //! `Comparable`
    //!
    //!
    //! @todo
    //! - Completely figure out and document the category theoretical
    //!   foundation of this data type.
    //! - Consider instantiating `Functor`, `Applicative` and `Monad` if
    //!   that's possible.
    //! - Consider having a `.name()` method that would return the
    //!   (demangled?) `typeid(T).name()`.
    //!
    //! @bug
    //! `metafunction` and friends are not SFINAE-friendly right now.
    //! See [this](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=59498) GCC bug
    //! and also [Core 1430](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430)
    //! issue. Once this issue is resolved, look at the unit tests for those
    //! utilities and either uncomment or remove the relevant test section.
    struct Type { };

    struct Metafunction { };

    namespace type_detail {
        template <typename T, typename ...Nothing>
        struct make_type {
            struct hidden : operators<Comparable> {
                using hana_datatype = Type;
                using type = T;

                template <typename ...Args>
                static constexpr auto construct(int, Args ...args)
                    -> decltype(T{args...})
                { return T{args...}; }

                template <typename ...Args>
                static constexpr auto construct(long, Args ...args)
                    -> decltype(T(args...))
                { return T(args...); }

                template <typename ...Args>
                constexpr auto operator()(Args ...args) const
                { return construct(int{0}, args...); }
            };
        };
    }

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
    //! @hideinitializer
    //!
    //! `type<T>` is a function returning an object of type `T` constructed
    //! with the arguments passed to it:
    //! @snippet example/type/construct.cpp main
    //!
    //! To use an initializer list, one should construct it explicitly:
    //! @snippet example/type/initializer_list.cpp main
    //!
    //! `decltype(type<T>)` also has a nested alias to `T` named `type`.
    //! Hence, it can be used as a metafunction returning `T`:
    //! @snippet example/type/as_metafunction.cpp main
    template <typename T>
    constexpr typename type_detail::make_type<T>::hidden type{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_TYPE_FWD_HPP
