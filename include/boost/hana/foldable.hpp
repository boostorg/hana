/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    /*!
    @typeclass{Foldable}
    @{
    The `Foldable` type class is used for data structures that can be folded.

    @mcd
    `foldl`, `foldl1`, `foldr1` and `lazy_foldr`

    @todo
    Provide examples for the methods? I think the instance of `Foldable` for
    `Iterable`s must be documented properly with examples, but we can't do
    better without loss of generality here. Or maybe we can use a parallel
    with `toList` to document equalities that must hold?
     */
    template <typename T>
    struct Foldable;

    //! Left-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl = [](auto f, auto state, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::foldl_impl(f, state, foldable);
    };

    //! Right-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr = [](auto f, auto state, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::foldr_impl(f, state, foldable);
    };

    //! Variant of `foldr` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr1 = [](auto f, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::foldr1_impl(f, foldable);
    };

    //! Variant of `foldl` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl1 = [](auto f, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::foldl1_impl(f, foldable);
    };

    //! Lazy right-associative fold of a structure using a binary operation.
    //!
    //! Unlike for strict folds, the binary operation should take nullary
    //! functions returning an element and the state instead of taking an
    //! element and the state directly.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy_foldr = [](auto f, auto state, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::lazy_foldr_impl(f, state, foldable);
    };

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum_by = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::minimum_by_impl(predicate, foldable);
    };

    //! Return the least element of a non-empty structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::minimum_impl(foldable);
    };

    //! Return the largest element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum_by = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::maximum_by_impl(predicate, foldable);
    };

    //! Return the largest element of a non-empty structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::maximum_impl(foldable);
    };

    //! Compute the sum of the numbers of a structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::sum_impl(foldable);
    };

    //! Compute the product of the numbers of a structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto product = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::product_impl(foldable);
    };

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto count = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::count_impl(predicate, foldable);
    };

    //! Invoke a function with the elements of a structure as arguments.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto unpack = [](auto f, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::unpack_impl(f, foldable);
    };

    //! Return whether any element of the structure satisfies the `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::any_impl(predicate, foldable);
    };

    //! Return whether any element of the structure is true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::any_of_impl(foldable);
    };

    //! Return whether all the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::all_impl(predicate, foldable);
    };

    //! Return whether all the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::all_of_impl(foldable);
    };

    //! Return whether none of the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto predicate, auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::none_impl(predicate, foldable);
    };

    //! Return whether none of the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto foldable) {
        return Foldable<datatype_t<decltype(foldable)>>::none_of_impl(foldable);
    };

    //! @}

    template <>
    struct defaults<Foldable> {
        template <typename F, typename State, typename Foldable_>
        static constexpr auto foldr_impl(F f, State s, Foldable_ foldable)
        { return lazy_foldr(fmap(apply, argwise(f)), s, foldable); }

        template <typename Foldable_>
        static constexpr auto minimum_impl(Foldable_ foldable)
        { return minimum_by([](auto x, auto y) { return x < y; }, foldable); }

        template <typename Foldable_>
        static constexpr auto maximum_impl(Foldable_ foldable)
        { return maximum_by([](auto x, auto y) { return x < y; }, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto minimum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(
                [=](auto x, auto y) { return if_(pred(x, y), x, y); },
                foldable
            );
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto maximum_by_impl(Pred pred, Foldable_ foldable) {
            return foldl1(
                [=](auto x, auto y) { return if_(pred(x, y), y, x); },
                foldable
            );
        }

        template <typename Foldable_>
        static constexpr auto sum_impl(Foldable_ foldable)
        { return foldl([](auto x, auto y) { return x + y; }, int_<0>, foldable); }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable)
        { return foldl([](auto x, auto y) { return x * y; }, int_<1>, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto count_impl(Pred pred, Foldable_ foldable) {
            auto inc = [=](auto counter, auto x) {
                return if_(pred(x), counter + size_t<1>, counter);
            };
            return foldl(inc, size_t<0>, foldable);
        }

        template <typename F, typename Foldable_>
        static constexpr auto unpack_impl(F f, Foldable_ foldable)
        { return foldl(partial, f, foldable)(); }

        template <typename Pred>
        struct lazy_or {
            Pred p;
            template <typename X, typename Y>
            constexpr auto operator()(X x, Y y) const
            { return call(p(x()), y); }

            template <typename Y>
            constexpr auto call(decltype(true_), Y y) const
            { return true_; }

            template <typename Y>
            constexpr auto call(decltype(false_), Y y) const
            { return y(); }

            template <typename Y>
            constexpr auto call(bool b, Y y) const
            { return b ? b : y(); }
        };

        // any, all, none
        template <typename Pred, typename Foldable_>
        static constexpr auto any_impl(Pred pred, Foldable_ foldable) {
            return lazy_foldr(lazy_or<Pred>{pred}, false_, foldable);
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto all_impl(Pred pred, Foldable_ foldable)
        { return !any([=](auto x) { return !pred(x); }, foldable); }

        template <typename Pred, typename Foldable_>
        static constexpr auto none_impl(Pred pred, Foldable_ foldable)
        { return !any(pred, foldable); }


        // any_of, all_of, none_of
        template <typename Foldable_>
        static constexpr auto any_of_impl(Foldable_ foldable)
        { return any(id, foldable); }

        template <typename Foldable_>
        static constexpr auto all_of_impl(Foldable_ foldable)
        { return all(id, foldable); }

        template <typename Foldable_>
        static constexpr auto none_of_impl(Foldable_ foldable)
        { return none(id, foldable); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
