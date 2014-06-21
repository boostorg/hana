/*!
@file
Defines `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/integral_fwd.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    `Functor` represents types that can be mapped over.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `fmap` or `adjust`

    --------------------------------------------------------------------------

    ## Laws
    Instances of `Functor` must satisfy the following laws:
    @code
        fmap id == id
        fmap (f . g) == fmap f . fmap g
    @endcode
     */
    template <typename T, typename Enable = void>
    struct Functor;

    //! Maps `f` over a `Functor`.
    //! @method{Functor}
    //!
    //! ### Fusion example
    //! @snippet example/list/functor/fmap.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/functor/fmap.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto fmap = [](auto f, auto functor) {
        return Functor<datatype_t<decltype(functor)>>::fmap_impl(f, functor);
    };

    //! Update all the elements satisfying the `predicate` with the given
    //! function.
    //! @method{Functor}
    //!
    //! ### Example
    //! @snippet example/list/functor/adjust.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto adjust = [](auto predicate, auto f, auto functor) {
        return Functor<datatype_t<decltype(functor)>>::adjust_impl(predicate, f, functor);
    };

    //! Replace all the elements satisfying the `predicate` with the given
    //! value.
    //! @method{Functor}
    //!
    //! ### Example
    //! @snippet example/list/functor/replace.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto replace = [](auto predicate, auto value, auto functor) {
        return Functor<datatype_t<decltype(functor)>>::replace_impl(predicate, value, functor);
    };

    template <>
    struct instance<Functor> {
        template <typename T, typename Enable = void>
        struct with { };
    };

    template <>
    struct defaults<Functor> {
        template <typename T, typename Enable = void>
        struct with : defaults<> {
            template <typename Pred, typename F, typename Foldable_>
            static constexpr auto adjust_impl(Pred pred, F f, Foldable_ foldable) {
                auto go = [=](auto x) {
                    return eval_if(pred(x),
                        [=](auto _) { return _(f)(x); },
                        [=](auto) { return x; }
                    );
                };
                return fmap(go, foldable);
            }

            template <typename Pred, typename Value, typename Foldable_>
            static constexpr auto replace_impl(Pred pred, Value v, Foldable_ foldable)
            { return adjust(pred, [=](auto) { return v; }, foldable); }

            template <typename F, typename Foldable_>
            static constexpr auto fmap_impl(F f, Foldable_ foldable)
            { return adjust([](auto) { return true_; }, f, foldable); }
        };
    };

    template <typename T, typename Enable>
    struct Functor : instance<Functor>::template with<T> { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
