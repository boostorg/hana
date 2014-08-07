/*!
@file
Defines the `boost::hana::List` instance.

@todo
Find a way to get rid of this header.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_INSTANCE_HPP
#define BOOST_HANA_LIST_INSTANCE_HPP

#include <boost/hana/list/list.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/variadic/at.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/range.hpp>


namespace boost { namespace hana {
    namespace list_detail {
        template <typename Storage, typename = operators::enable>
        struct list {
            using hana_datatype = List;
            Storage storage;
        };
    }

    //! Creates a `List` containing `xs...`.
    //! @relates List
    //!
    //! The `List` data type is a general purpose compile-time heterogeneous
    //! sequence.
    //!
    //! @todo
    //! Consider having `list_of<Type>`, `list_of<Integral>`, ...,
    //! `list_of<Anything>`, with `list == list_of<Anything>`. It does not
    //! fix the problem of partial type classes (e.g. `MplVector` is not
    //! _actually_ a `List`), but at least we remove `TypeList` and
    //! `IntegerList`, which are arguably ugly.
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = [](auto ...xs) {
        auto storage = [=](auto f) { return f(xs...); };
        return list_detail::list<decltype(storage)>{storage};
    };

    //! @cond
    template <>
    struct Foldable::instance<List> : Foldable::unpack_mcd {
        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs xs, F f) {
            return xs.storage(f);
        }
    };
    //! @endcond

    //! @details
    //! `List` is an `Iterable` in the most obvious way. The head of a
    //! non-empty list corresponds to its first element. The tail of a
    //! non-empty list is a list containing all the elements in the same
    //! order, except the head. Finally, a list is empty if and only if
    //! it has no elements in it.
    //!
    //! ### Example
    //! @snippet example/list/iterable/overview.cpp main
    template <>
    struct Iterable::instance<List> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return x;
            });
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return xs.storage([](auto x, auto ...rest) {
                return list(rest...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            return xs.storage([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }

        template <typename Index, typename Xs>
        static constexpr auto at_impl(Index n, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return detail::variadic::at<n()>(xs...);
            });
        }
    };

    //! Instance of `List` for the `List` data type.
    template <>
    struct List::instance<List> : List::mcd<List> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto nil_impl() {
            return list();
        }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return list(x, xs...);
            });
        }

        template <typename Xs, typename Ys>
        static constexpr auto concat_impl(Xs xs, Ys ys) {
            return xs.storage([=](auto ...xs) {
                return ys.storage([=](auto ...ys) {
                    return list(xs..., ys...);
                });
            });
        }

        template <typename Xs>
        static constexpr auto init_impl(Xs xs) {
            return unpack(range(size_t<0>, length(xs) - size_t<1>),
                on(list, [=](auto index) { return at(index, xs); })
            );
        }

        template <typename Xs, typename X>
        static constexpr auto snoc_impl(Xs xs, X x) {
            return xs.storage([=](auto ...xs) {
                return list(xs..., x);
            });
        }

        template <typename N, typename Xs>
        static constexpr auto take_impl(N n, Xs xs) {
            return unpack(range(size_t<0>, min(n, length(xs))),
                on(list, [=](auto index) { return at(index, xs); })
            );
        }

        template <typename F, typename ...Xss>
        static constexpr auto zip_with_impl(F f, Xss ...lists) {
            auto go = [=](auto index, auto ...nothing) {
                return always(f)(nothing...)(at(index, lists)...);
            };
            auto zip_length = minimum(list(length(lists)...));
            return unpack(range(size_t<0>, zip_length),
                on(list, go)
            );
        }
    };

    //! @cond
    template <>
    struct Functor::instance<List> : Functor::fmap_mcd {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return list(f(xs)...);
            });
        }
    };
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_INSTANCE_HPP
