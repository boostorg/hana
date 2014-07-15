/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/maybe_fwd.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Data structures that can be folded, i.e. summarized into
    //! a single value.
    struct Foldable {
        BOOST_HANA_TYPECLASS(Foldable);
        struct lazy_foldr_mcd;
    };

    //! Left-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldl.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/foldl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl = [](auto f, auto state, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::foldl_impl(f, state, foldable);
    };

    //! Right-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/foldr.cpp main
    //!
    //! ### Example 2
    //! @snippet example/integer_list/foldable/foldr.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr = [](auto f, auto state, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::foldr_impl(f, state, foldable);
    };

    //! Variant of `foldr` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/foldr1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldr1 = [](auto f, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::foldr1_impl(f, foldable);
    };

    //! Variant of `foldl` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/foldl1.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldl1 = [](auto f, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::foldl1_impl(f, foldable);
    };

    //! Lazy right-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! Unlike for strict folds, the binary operation should take nullary
    //! functions returning an element and the state instead of taking an
    //! element and the state directly.
    //!
    //! ### Example
    //! @snippet example/range/foldable/lazy_foldr.cpp main
    //!
    //! @bug
    //! We got a performance problem here. It seems very hard to implement
    //! this method efficiently.
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy_foldr = [](auto f, auto state, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::lazy_foldr_impl(f, state, foldable);
    };

    //! Lazy left-associative fold of a structure using a binary operation.
    //! @method{Foldable}
    //!
    //! Unlike for strict folds, the binary operation should take nullary
    //! functions returning the state and an element instead of taking the
    //! state and an element directly.
    //!
    //! @warning
    //! It is important to be aware that a lazy left fold must still walk the
    //! whole structure before it can return. This is because of the nature of
    //! left-folds, which are always equivalent to
    //! @code
    //!     foldl(f, state, structure):
    //!         if (some_stop_condition)
    //!             return state
    //!         else
    //!             return foldl(f, f(...), ...);
    //! @endcode
    //! Notice how `foldl` calls itself recursively in the `else` branch; this
    //! means that the next invocation of `foldl` is always needed, and so the
    //! whole structure has to be unfolded. When `f` is lazy, this has the
    //! effect of creating a (potentially huge) chain of "thunks":
    //! @code
    //!     f(f(f(f(f(x1, state), x2), x3), x4), x5)
    //! @endcode
    //! This chain is then only evaluated lazily, but creating the chain
    //! itself can cause a stack overflow. If you don't need to accumulate
    //! the result lazily, consider using `foldl` instead, which does not
    //! create a chain of thunks and evaluates `f` as it goes.
    //!
    //! ### Example
    //! @snippet example/range/foldable/lazy_foldl.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy_foldl = [](auto f, auto state, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::lazy_foldl_impl(f, state, foldable);
    };

    //! Return the number of elements in a finite structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/length.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto length = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::length_impl(foldable);
    };

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/type_list/foldable/minimum_by.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::minimum_by_impl(predicate, foldable);
    };

    //! Return the least element of a non-empty structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/integer_list/foldable/minimum.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimum = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::minimum_impl(foldable);
    };

    //! Return the largest element of a non-empty structure with respect to
    //! a `predicate`.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum_by = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::maximum_by_impl(predicate, foldable);
    };

    //! Return the largest element of a non-empty structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto maximum = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::maximum_impl(foldable);
    };

    //! Compute the sum of the numbers of a structure.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto sum = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::sum_impl(foldable);
    };

    //! Compute the product of the numbers of a structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/range/foldable/product.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto product = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::product_impl(foldable);
    };

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @method{Foldable}
    //!
    //! ### Example 1
    //! @snippet example/integer_list/foldable/count.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/count.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto count = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::count_impl(predicate, foldable);
    };

    //! Find an element satisfying a predicate in the structure.
    //! @method{Foldable}
    //!
    //! Specifically, returns `just` the first element satisfying the
    //! `predicate`, or `nothing` if there is no such element.
    //!
    //! ### Example 1
    //! @snippet example/list/foldable/find.cpp main
    //!
    //! ### Example 2
    //! @snippet example/type_list/foldable/find.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto find = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::find_impl(predicate, foldable);
    };

    //! Return whether the element occurs in the structure.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/elem.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto elem = [](auto x, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::elem_impl(x, foldable);
    };

    //! Invoke a function with the elements of a structure as arguments.
    //! @method{Foldable}
    //!
    //! ### Example
    //! @snippet example/list/foldable/unpack.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto unpack = [](auto f, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::unpack_impl(f, foldable);
    };

    //! Return whether any element of the structure satisfies the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/any.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/any.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::any_impl(predicate, foldable);
    };

    //! Return whether any element of the structure is true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::any_of_impl(foldable);
    };

    //! Return whether all the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/all.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/all.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::all_impl(predicate, foldable);
    };

    //! Return whether all the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::all_of_impl(foldable);
    };

    //! Return whether none of the elements of the structure satisfy the `predicate`.
    //! @method{Foldable}
    //!
    //! ### Fusion example
    //! @snippet example/list/foldable/none.cpp fusion
    //!
    //! ### MPL example
    //! @snippet example/list/foldable/none.cpp mpl
    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto predicate, auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::none_impl(predicate, foldable);
    };

    //! Return whether none of the elements of the structure are true-valued.
    //! @method{Foldable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto foldable) {
        return Foldable::instance<datatype_t<decltype(foldable)>>::none_of_impl(foldable);
    };

    //! @details
    //! Minimal complete definition: `lazy_foldr`
    struct Foldable::lazy_foldr_mcd {
        template <typename F, typename State, typename Foldable_>
        static constexpr auto foldr_impl(F f, State s, Foldable_ foldable)
        { return lazy_foldr(on(f, apply), s, foldable); }

        template <typename F, typename Foldable_>
        static constexpr auto foldr1_impl(F f, Foldable_ foldable) {
            auto maybe_f = [=](auto lx, auto my) {
                return just(maybe(lx(), partial(f, lx()), my()));
            };
            return from_just(lazy_foldr(maybe_f, nothing, foldable));
        }

        template <typename F, typename State, typename Foldable_>
        static constexpr auto foldl_impl(F f, State s, Foldable_ foldable) {
            auto f_ = [=](auto x, auto next) {
                return [=](auto state) {
                    return next()(f(state, x()));
                };
            };
            return lazy_foldr(f_, id, foldable)(s);
        }

        template <typename F, typename Foldable_>
        static constexpr auto foldl1_impl(F f, Foldable_ foldable) {
            auto maybe_f = [=](auto mx, auto ly) {
                return maybe(
                    just(ly()),
                    [=](auto x) { return just(f(x, ly())); },
                    mx()
                );
            };
            return from_just(lazy_foldl(maybe_f, nothing, foldable));
        }

        template <typename F, typename State, typename Foldable_>
        static constexpr auto lazy_foldl_impl(F f, State s, Foldable_ foldable) {
            auto f_ = [=](auto lx, auto lnext) {
                return [=](auto state) {
                    return lnext()(f([=] { return state; }, lx));
                };
            };
            return lazy_foldr(f_, id, foldable)(s);
        }

        template <typename Foldable_>
        static constexpr auto length_impl(Foldable_ foldable)
        { return foldl(_ + size_t<1>, size_t<0>, foldable); }

        template <typename Foldable_>
        static constexpr auto minimum_impl(Foldable_ foldable)
        { return minimum_by(less, foldable); }

        template <typename Foldable_>
        static constexpr auto maximum_impl(Foldable_ foldable)
        { return maximum_by(less, foldable); }

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

        template <typename Pred, typename Foldable_>
        static constexpr auto find_impl(Pred pred, Foldable_ foldable) {
            auto go = [=](auto x, auto tail) {
                return eval_if(pred(x()),
                    always(just(x())),
                    [=](auto id) { return id(tail)(); }
                );
            };
            return lazy_foldr(go, nothing, foldable);
        }

        template <typename X, typename Foldable_>
        static constexpr auto elem_impl(X x, Foldable_ foldable)
        { return any([=](auto y) { return equal(x, y); }, foldable); }

        template <typename Foldable_>
        static constexpr auto sum_impl(Foldable_ foldable)
        { return foldl(_ + _, int_<0>, foldable); }

        template <typename Foldable_>
        static constexpr auto product_impl(Foldable_ foldable)
        { return foldl(_ * _, int_<1>, foldable); }

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


        // any, all, none
        template <typename Pred, typename Foldable_>
        static constexpr auto any_impl(Pred pred, Foldable_ foldable) {
            auto lazy_or = [=](auto lx, auto ly) {
                auto p = pred(lx());
                return eval_if(p, always(p), [=](auto _) { return _(ly)(); });
            };
            return lazy_foldr(lazy_or, false_, foldable);
        }

        template <typename Pred, typename Foldable_>
        static constexpr auto all_impl(Pred pred, Foldable_ foldable)
        { return not_(any([=](auto x) { return not_(pred(x)); }, foldable)); }

        template <typename Pred, typename Foldable_>
        static constexpr auto none_impl(Pred pred, Foldable_ foldable)
        { return not_(any(pred, foldable)); }


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
