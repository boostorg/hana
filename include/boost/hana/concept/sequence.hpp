/*!
@file
Defines `boost::hana::Sequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_SEQUENCE_HPP
#define BOOST_HANA_CONCEPT_SEQUENCE_HPP

#include <boost/hana/fwd/concept/sequence.hpp>

#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/by.hpp> // needed by xxx.by
#include <boost/hana/insert.hpp>
#include <boost/hana/detail/variadic/foldr1.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/concept/product.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/tuple.hpp>

#include <boost/hana/cartesian_product.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/insert_range.hpp>
#include <boost/hana/intersperse.hpp>
#include <boost/hana/partition.hpp>
#include <boost/hana/permutations.hpp>
#include <boost/hana/remove_at.hpp>
#include <boost/hana/reverse.hpp>
#include <boost/hana/scan_left.hpp>
#include <boost/hana/scan_right.hpp>
#include <boost/hana/slice.hpp>
#include <boost/hana/sort.hpp>
#include <boost/hana/span.hpp>
#include <boost/hana/subsequence.hpp>
#include <boost/hana/take.hpp>
#include <boost/hana/take_while.hpp>
#include <boost/hana/unfold_left.hpp>
#include <boost/hana/unfold_right.hpp>
#include <boost/hana/unique.hpp>
#include <boost/hana/zip.hpp>
#include <boost/hana/zip_shortest.hpp>
#include <boost/hana/zip_shortest_with.hpp>
#include <boost/hana/zip_with.hpp>

#include <cstddef>
#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Comparable
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename Xs, typename Ys, std::size_t Length>
        struct compare_finite_sequences {
            Xs const& xs;
            Ys const& ys;

            template <std::size_t i>
            constexpr auto apply(decltype(hana::false_), decltype(hana::true_)) const {
                return compare_finite_sequences::apply<i+1>(
                    hana::bool_<i+1 == Length>,
                    hana::if_(hana::equal(hana::at_c<i>(xs), hana::at_c<i>(ys)),
                              hana::true_, hana::false_)
                );
            }

            template <std::size_t i>
            constexpr auto apply(decltype(hana::false_), decltype(hana::false_)) const
            { return hana::false_; }

            template <std::size_t i, typename Result>
            constexpr auto apply(decltype(hana::true_), Result r) const
            { return r; }

            template <std::size_t i>
            constexpr bool apply(decltype(hana::false_), bool b) const {
                return b && compare_finite_sequences::apply<i+1>(
                    hana::bool_<i+1 == Length>,
                    hana::if_(hana::equal(hana::at_c<i>(xs), hana::at_c<i>(ys)),
                              hana::true_, hana::false_)
                );
            }
        };
    }

    template <typename T, typename U>
    struct Sequence::equal_impl {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs const& xs, Ys const& ys) {
            constexpr std::size_t xs_size = hana::value<decltype(hana::length(xs))>();
            constexpr std::size_t ys_size = hana::value<decltype(hana::length(ys))>();
            detail::compare_finite_sequences<Xs, Ys, xs_size> comp{xs, ys};
            return comp.template apply<0>(hana::bool_<xs_size == 0>,
                                          hana::bool_<xs_size == ys_size>);
        }
    };

    template <typename T, typename U>
    struct equal_impl<T, U, when<_models<Sequence, T>::value && _models<Sequence, U>::value>>
        : Sequence::equal_impl<T, U>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct Sequence::less_impl {
        template <typename Xs, typename Ys>
        static constexpr auto
        less_helper(Xs const&, Ys const&, decltype(true_), decltype(true_))
        { return false_; }

        template <typename Xs, typename Ys>
        static constexpr auto
        less_helper(Xs const&, Ys const&, decltype(true_), decltype(false_))
        { return true_; }

        template <typename Xs, typename Ys>
        static constexpr auto
        less_helper(Xs const&, Ys const&, decltype(false_), decltype(true_))
        { return false_; }

        template <typename Xs, typename Ys>
        static constexpr auto
        less_helper(Xs const& xs, Ys const& ys, decltype(false_), decltype(false_)) {
            return hana::or_(
                hana::less(hana::front(xs), hana::front(ys)),
                hana::and_(
                    hana::equal(hana::front(xs), hana::front(ys)),
                    less_impl::apply(hana::tail(xs), hana::tail(ys))
                )
            );
        }

        template <typename Xs, typename Ys>
        static constexpr bool
        less_helper(Xs const& xs, Ys const& ys, bool e_xs, bool e_ys) {
                 if (e_xs && e_ys) return false;
            else if (e_xs && !e_ys) return true;
            else if (!e_xs && e_ys) return false;

            return hana::or_(
                hana::less(hana::front(xs), hana::front(ys)),
                hana::and_(
                    hana::equal(hana::front(xs), hana::front(ys)),
                    less_impl::apply(hana::tail(xs), hana::tail(ys))
                )
            );
        }

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs const& xs, Ys const& ys) {
            return less_helper(xs, ys,
                hana::if_(hana::is_empty(xs), true_, false_),
                hana::if_(hana::is_empty(ys), true_, false_));
        }
    };

    template <typename T, typename U>
    struct less_impl<T, U, when<_models<Sequence, T>::value && _models<Sequence, U>::value>>
        : Sequence::less_impl<T, U>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Applicative
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct Sequence::lift_impl {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::make<S>(static_cast<X&&>(x)); }
    };

    template <typename S>
    struct ap_impl<S, when<_models<Sequence, S>::value>>
        : Monad::ap_impl<S>
    { };

    template <typename S>
    struct lift_impl<S, when<_models<Sequence, S>::value>>
        : Sequence::lift_impl<S>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Automatic model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct find_if_impl<S, when<_models<Sequence, S>::value &&
                                !_models<Foldable, S>::value>>
        : Iterable::find_if_impl<S>
    { };

    template <typename S>
    struct any_of_impl<S, when<_models<Sequence, S>::value &&
                               !_models<Foldable, S>::value>>
        : Iterable::any_of_impl<S>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_SEQUENCE_HPP
