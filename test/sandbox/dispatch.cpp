/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

/*
minimal goals:
- be able to provide default methods
- be able to enable type classes based on an arbitrary predicate
- the above predicates should not conflict with non-predicated but better-matching specializations (requires overloading?)


note:
I think it's possible to use these constructions to prove theorems?
 */

#include <boost/hana.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <iostream>
#include <typeinfo>



//////////////////////////////////////////////////////////////////////////////
#include <cxxabi.h>
#include <memory>
#include <string>
std::string demangle(char const* mangled) {
    int status;
    std::unique_ptr<char[], void (*)(void*)> result(
      abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
    return result.get() ? std::string(result.get()) : "error occurred";
}
//////////////////////////////////////////////////////////////////////////////


namespace bi {
    //////////////////////////////////////////////////////////////////////////
    // Core
    //////////////////////////////////////////////////////////////////////////

    using boost::hana::datatype_t;

    /*
    Default provided methods (what's not in the MCD).

    As it turns out this is a simple proof of the following statement:

        hyp1, ..., hypN => conclusion

    In our case, hypotheses are type class instantiations and the conclusion
    is a new type class instantiation. The implementation is the proof. When
    viewed in this regard, `defaults` is nothing but

        mcd => typeclass

    which is basically saying: if you give me the minimal complete definition
    then I can give you the whole type class. It would also make sense to have
    stuff like

        Monad, Iterable => SomethingElse

    and that should be supported.
     */
    template <template <typename ...> class Typeclass, typename ...T>
    struct defaults { };

    //! Specialize to instantiate a type class.
    //!
    //! In the theorem proving parallel, that would be like providing an axiom
    //! if the instance does not depend on anything and like providing a proof
    //! to a theorem if the instance depends on other axioms.
    template <template <typename ...> class Typeclass, typename ...T>
    struct instance { };

    /*!
    Like `instance`, but with a lower priority. When resolving methods,
    `instance` is checked before `automatic_instance` is checked.

    This allows a set of type classes called the Providers to provide an
    instance for a type class called the Provided. `automatic_instance`
    must be disabled using a predicate if the user did not explicitly
    opt-in an automatic instantiation unless you want to break the world.
    @code
        template <typename T>
        struct automatic_instance<some_predicate, Typeclass, T> { whatever };
    @endcode
     */
    template <bool, template <typename ...> class Typeclass, typename ...T>
    struct automatic_instance { };


    namespace hooks {
        template <typename ...>
        struct datatypes { };

        template <template <typename ...> class>
        struct typeclass { };

        //! Type class specific validations.
        template <template <typename ...> class Typeclass, typename ...T>
        BOOST_HANA_CONSTEXPR_LAMBDA auto validations = [] {
            std::cerr << "validations running for "
                      << demangle(typeid(typeclass<Typeclass>).name())
                      << " with "
                      << demangle(typeid(datatypes<T...>).name())
                      << '\n';
        };

        //! Hook called when no instance is found for a given type class.
        template <template <typename ...> class Typeclass, typename ...T>
        BOOST_HANA_CONSTEXPR_LAMBDA auto no_instance = [] {
            std::cerr << "no instance of "
                      << demangle(typeid(typeclass<Typeclass>).name())
                      << " found for "
                      << demangle(typeid(datatypes<T...>).name())
                      << '\n';
        };
    }

    template <template <typename ...> class Typeclass, typename ...T>
    struct _dispatch {
        template <typename Fetcher>
        static constexpr auto call(Fetcher fetch_impl, int)
            -> decltype(fetch_impl(instance<Typeclass, T...>{}))
        { return fetch_impl(instance<Typeclass, T...>{}); }

        template <typename Fetcher>
        static constexpr auto call(Fetcher fetch_impl, long)
            -> decltype(fetch_impl(automatic_instance<true, Typeclass, T...>{}))
        { return fetch_impl(automatic_instance<true, Typeclass, T...>{}); }

        template <typename Fetcher>
        static constexpr auto call(Fetcher fetch_impl, void*)
            -> decltype(fetch_impl(defaults<Typeclass, T...>{}))
        { return fetch_impl(defaults<Typeclass, T...>{}); }

        template <typename Fetcher>
        static constexpr auto call(Fetcher fetch_impl, ...)
        { return hooks::no_instance<Typeclass, T...>(); }

        template <typename Fetcher>
        constexpr auto operator()(Fetcher fetch_impl) const {
            hooks::validations<Typeclass, T...>();
            return call(fetch_impl, (int)0);
        }
    };

    /*!
    Dispatch a type class method to the correct implementation.

    Right now, the correct implementation is simply defined as the most specific
    implementation that is explicitly enabled by the user.


    ### Algorithm
    1. If there is no instance for the given type class and data type,
       use the no_instance hook.

    2. Otherwise, try fetching the implementation from the instance<>
       specialization, which contains the most specific implementation
       overrides.

    3. If this fails, there are two possible reasons:
       (a) The user did not specialize `instance` and is relying on automatic
           instantiation.
       (b) `instance` is specialized but simply does not contain the method
           we're looking for. In this case, the user may or may not be relying
           on automatic instantiation.

       Since automatic instantiations are only enabled when users opt-in
       explicitly, we can just check whether the automatic instantiation
       contains our method. If that's the case, we are done. Otherwise,

       (a) There exists no automatic specialization at all.
       (b) There exists one but it's disabled because the user did not opt-in.
       (c) There exists one and the user opted-in but it does not contain our
           method because it relies on the default behavior.

       In all cases, we must go to the next step.

    4. If all else fails, we fall back to the default implementation.

     */
    template <template <typename ...> class Typeclass, typename ...T>
    constexpr _dispatch<Typeclass, T...> dispatch{};

#if 0
    template <template <typename ...> class Typeclass, typename ...T>
    auto dispatch = [](auto fetch_impl) {
        if (!is_instance<Typeclass, T>)
            throw no_instance<Typeclass, T>;

        validations<Typeclass, T>; // can this cause a recursive call to dispatch? probably
        try {
            return fetch_impl(instance<Typeclass, T>);
        } catch (invalid expression) {
            try {
                return fetch_impl(automatic_instance<Typeclass, T>);
            } catch (invalid expression) {
                try {
                    return fetch_impl(defaults<Typeclass, T>);
                } catch (invalid expression) {
                    THERES A BUG IN YOUR TYPE CLASS
                }
            }
        }
    };
#endif


    #if 0

    /*
    Whether there exists an instance of the given Typeclass for the given data type(s).

    There exists an instance iff any of the following holds
        (1) A mcd (and perhaps more methods) was given by specializing `instance`.

        ????????????????? validate point (2)
        (2) There exists an automatic instantiation of `Typeclass` by a type class
            instantiated by `T`, __and__ `T` has opted-in that automatic instantiation.
     */
    template <template <typename ...> class Typeclass, typename ...T>
    constexpr auto is_instance =
        trait::is_default_constructible(type<instance<Typeclass, T...>>) ||
        ????????????????
    ;

    /*
    Returns whether the data type has opted-in the instantiation provided by
    the provider. It is assumed that the provider _can_ provide that instantiation.

    In the theorem proving parallel, we're given

        Provider => Provided

    as an axiom (hence the assertion below) and we want to know whether `Provided`
    holds. To do that, we check whether `Provider` holds (`is_instance<Provider, T>`).
    This is equivalent to a modus ponens. We also check whether we explicitly
    enabled this inference rule, which simply gives us a customization point:

        instance<Provider, T>::enable_automatic_instance<Provided>

     */
    template <template <typename ...> class Provider, template <typename ...> class Provided, typename T>
    constexpr auto wants_automatic_instance =
        assert(provides<Provider, Provided>) &&
        is_instance<Provider, T> &&
        instance<Provider, T>::enable_automatic_instance<Provided>;

    // whether an instance of the provider can be made an instance of the provided. math: returns whether (Provider => Provided) holds
    template <template <typename ...> class Provider, template <typename ...> class Provided>
    constexpr auto provides = magic;

    #endif



    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////

    template <typename T>
    struct Iterable;

    template <typename T>
    struct defaults<Iterable, T> : boost::hana::defaults<boost::hana::Iterable> {

    };

    // template <typename T, typename U>
    // struct automatic_instance<Comparable, T, U, wants_provided<Iterable, Comparable, T> &&
    //                                             wants_provided<Iterable, Comparable, U>>
    // { comparison of iterables };

    // template <typename T>
    // struct automatic_instance<Foldable, T, wants_provided<Iterable, Foldable, T>>
    // { fold using iterable methods };


    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto xs) {
        return dispatch<Iterable, datatype_t<decltype(xs)>>(
            [=](auto It) -> decltype(It.head_impl(xs)) {
                return It.head_impl(xs);
            });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto xs) {
        return dispatch<Iterable, datatype_t<decltype(xs)>>(
            [=](auto It) -> decltype(It.tail_impl(xs)) {
                return It.tail_impl(xs);
            });
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto xs) {
        return dispatch<Iterable, datatype_t<decltype(xs)>>(
            [=](auto It) -> decltype(It.is_empty_impl(xs)) {
                return It.is_empty_impl(xs);
            });
    };


    //////////////////////////////////////////////////////////////////////////
    // List
    //////////////////////////////////////////////////////////////////////////
    using boost::hana::list;
    using boost::hana::List;

    template <>
    struct instance<Iterable, List> {
        // static constexpr auto enable_automatic_instance = list(Comparable, Foldable);

        template <typename Xs>
        static constexpr auto head_impl(Xs xs)
        { return boost::hana::head(xs); }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return boost::hana::is_empty(xs); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs)
        { return boost::hana::tail(xs); }
    };

    #if 0
    template <>
    struct instance<Comparable, List, List> {
        // something much more efficient
    };

    template <>
    struct instance<Foldable, List> {
        // We have a super fast unpack, but we'd like to let Iterable provide
        // the rest of the methods. How can we do that?
        //
        // Hmm, it turns out that we already _asked explicitly_ for an instance of
        // Foldable to Iterable above. We just pulled the rug under our feet by
        // specializing `instance<...>`, which unfortunately _happens_ to be the
        // way to provide automatic instances too. By adding one more layer
        // which is specific to automatic instances, we could specialize
        // `instance` and still get the automatic behavior as a fallback.
        //
        // (VERIFY THIS) This also has the effect of allowing parameterized data
        // types instantiations because there won't be an ambiguous specialization
        // with the automatic (predicated) instantiations.
        //
        // ----------------------------------------------
        //
        // Also, suppose we have two possible mcds:
        //  (1) lazy_foldr + foldl
        //  (2) unpack + foldl
        //
        // The user must be able to pick between both mcds and we can't just make
        // the choice ourselves because we don't know whether unpack in (1) and
        // lazy_foldr in (2) can be implemented (what if we're in the wrong mcd?).
        //
        // Also, suppose that we'd like to provide different implementations
        // depending on some performance properties of some operation. For example,
        // we might decide to implement `sum` in a manner (X) if we can `unpack`
        // efficiently and in a manner (Y) if we can't.
        //
        // So the type class implementer needs a way to see the mcd when it
        // creates the default methods.

        static constexpr auto unpack_is_fast = true_;
        static constexpr auto unpack_impl(...) { ... }

        // +Feature
        // Even if that's not part of the mcd, it should be picked up.
        static constexpr auto any_of(...) { ... }
    };
    #endif
} // end namespace bi

using namespace bi;

int main() {
    BOOST_HANA_STATIC_ASSERT(head(list(1, 2)) == 1);
    BOOST_HANA_STATIC_ASSERT(tail(list(1, 2, 3)) == list(2, 3));
    BOOST_HANA_STATIC_ASSERT(!is_empty(list(1, 2, 3)));
    head(1);
}


#if 0

namespace cozy_ns {
    struct MyList {
        struct Iterable {
            template <typename Xs>
            static constexpr auto head_impl(Xs xs)
            { return whatever; }
        };

        using Foldable = boost::hana::from<boost::hana::Iterable>;
        using Comparable = boost::hana::from<boost::hana::Iterable>;
    };
}


#endif



#if 0

template <typename T>
struct Iterable : IterableAuto<T> { };

template <typename T>
struct IterableAuto : IterableDefaults<T> { };

template <typename T>
struct IterableDefaults { };

#endif
