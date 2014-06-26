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
    BOOST_HANA_TYPECLASS_BOILERPLATE(struct Functor)

    /*!
    @ingroup typeclasses
    `Functor` represents types that can be mapped over.

    --------------------------------------------------------------------------

    ## Laws
    Instances of `Functor` must satisfy the following laws:
    @code
        fmap id == id
        fmap (f . g) == fmap f . fmap g
    @endcode
     */
    struct Functor : typeclass<Functor> {
        struct fmap_mcd;
        struct adjust_mcd;
    };

    //! Maps `f` over a `Functor`.
    //! @method{Functor}
    //!
    //! ### Example 1
    //! @snippet example/list/functor/fmap.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/functor/fmap.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto fmap = [](auto f, auto functor) {
        return Functor::instance<datatype_t<decltype(functor)>>::fmap_impl(f, functor);
    };

    //! Update all the elements satisfying the `predicate` with the given
    //! function.
    //! @method{Functor}
    //!
    //! ### Example
    //! @snippet example/list/functor/adjust.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto adjust = [](auto predicate, auto f, auto functor) {
        return Functor::instance<datatype_t<decltype(functor)>>::adjust_impl(predicate, f, functor);
    };

    //! Replace all the elements satisfying the `predicate` with the given
    //! value.
    //! @method{Functor}
    //!
    //! ### Example
    //! @snippet example/list/functor/replace.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto replace = [](auto predicate, auto value, auto functor) {
        return Functor::instance<datatype_t<decltype(functor)>>::replace_impl(predicate, value, functor);
    };

    //! Replace all the elements with a value.
    //! @method{Functor}
    //!
    //! ### Example
    //! @snippet example/list/functor/fill.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto fill = [](auto value, auto functor) {
        return Functor::instance<datatype_t<decltype(functor)>>::fill_impl(value, functor);
    };

    namespace functor_detail {
        struct common {
            template <typename Pred, typename Value, typename F>
            static constexpr auto replace_impl(Pred pred, Value v, F functor)
            { return adjust(pred, [=](auto) { return v; }, functor); }

            template <typename Value, typename F>
            static constexpr auto fill_impl(Value v, F functor)
            { return fmap([=](auto) { return v; }, functor); }
        };
    }

    //! Minimal complete definition: `fmap`
    struct Functor::fmap_mcd : functor_detail::common {
        template <typename Pred, typename F, typename Functor_>
        static constexpr auto adjust_impl(Pred pred, F f, Functor_ functor) {
            auto go = [=](auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return _(f)(x); },
                    [=](auto) { return x; }
                );
            };
            return fmap(go, functor);
        }
    };

    //! Minimal complete definition: `adjust`
    struct Functor::adjust_mcd : functor_detail::common {
        template <typename F, typename Functor_>
        static constexpr auto fmap_impl(F f, Functor_ functor)
        { return adjust([](auto) { return true_; }, f, functor); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
