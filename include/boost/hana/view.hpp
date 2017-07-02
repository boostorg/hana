/*!
@file
Defines `boost::hana::view`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_VIEW_HPP
#define BOOST_HANA_VIEW_HPP

#include <boost/hana/fwd/view.hpp>

#include <boost/hana/all_of.hpp>
#include <boost/hana/and.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/cartesian_product.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/decay.hpp>
#include <boost/hana/detail/unpack_flatten.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/fwd/ap.hpp>
#include <boost/hana/fwd/concat.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/drop_front.hpp>
#include <boost/hana/fwd/empty.hpp>
#include <boost/hana/fwd/flatten.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/less.hpp>
#include <boost/hana/fwd/lift.hpp>
#include <boost/hana/fwd/transform.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/lexicographical_compare.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    namespace detail {
        template <typename Sequence>
        struct is_view {
            static constexpr bool value = false;
        };

        template <typename Sequence>
        using view_storage = typename std::conditional<
            detail::is_view<Sequence>::value, Sequence, Sequence&
        >::type;

        //////////////////////////////////////////////////////////////////////
        // cartesian_product_element_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequences, std::size_t i>
        struct cartesian_product_element_view_t {
            detail::view_storage<Sequences> sequences_;
            using hana_tag = view_tag;
        };

        template <typename Sequences, std::size_t i>
        struct is_view<cartesian_product_element_view_t<Sequences, i>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // cartesian_product_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequences>
        struct cartesian_product_view_t {
            detail::view_storage<Sequences> sequences_;
            using hana_tag = view_tag;
        };

        struct make_cartesian_product_view_t {
            template <typename Sequences>
            constexpr cartesian_product_view_t<Sequences> operator()(Sequences& s) const {
                return {s};
            }
        };

        constexpr make_cartesian_product_view_t cartesian_product_view{};

        template <typename Sequences>
        struct is_view<cartesian_product_view_t<Sequences>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // sliced_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequence, std::size_t ...indices>
        struct sliced_view_t {
            detail::view_storage<Sequence> sequence_;
            using hana_tag = view_tag;
        };

        template <typename Sequence, typename Indices>
        constexpr auto sliced(Sequence& sequence, Indices const& indices) {
            return hana::unpack(indices, [&](auto ...i) {
                return sliced_view_t<Sequence, decltype(i)::value...>{sequence};
            });
        }

        template <typename Sequence, std::size_t ...i>
        struct is_view<sliced_view_t<Sequence, i...>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // transformed_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequence, typename F>
        struct transformed_view_t {
            detail::view_storage<Sequence> sequence_;
            F f_;
            using hana_tag = view_tag;
        };

        template <typename Sequence, typename F>
        constexpr transformed_view_t<Sequence, typename hana::detail::decay<F>::type>
        transformed(Sequence& sequence, F&& f) {
            return {sequence, static_cast<F&&>(f)};
        }

        template <typename Sequence, typename F>
        struct is_view<transformed_view_t<Sequence, F>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // joined_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequence1, typename Sequence2>
        struct joined_view_t {
            detail::view_storage<Sequence1> sequence1_;
            detail::view_storage<Sequence2> sequence2_;
            using hana_tag = view_tag;
        };

        struct make_joined_view_t {
            template <typename Sequence1, typename Sequence2>
            constexpr joined_view_t<Sequence1, Sequence2>
            operator()(Sequence1& s1, Sequence2& s2) const {
                return {s1, s2};
            }
        };
        constexpr make_joined_view_t joined{};

        template <typename Sequence1, typename Sequence2>
        struct is_view<joined_view_t<Sequence1, Sequence2>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // flattened_view
        //////////////////////////////////////////////////////////////////////
        template <typename Sequences>
        struct flattened_view_t {
            detail::view_storage<Sequences> sequences_;
            using hana_tag = view_tag;
        };

        struct make_flattened_view_t {
            template <typename Sequences>
            constexpr flattened_view_t<Sequences> operator()(Sequences& s) const {
                return {s};
            }
        };
        constexpr make_flattened_view_t flattened{};

        template <typename Sequences>
        struct is_view<flattened_view_t<Sequences>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // single_view
        //////////////////////////////////////////////////////////////////////
        template <typename T>
        struct single_view_t {
            T& value_;
            using hana_tag = view_tag;
        };

        template <typename T>
        constexpr single_view_t<T> single_view(T& t)
        { return {t}; }

        template <typename T>
        struct is_view<single_view_t<T>> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // empty_view
        //////////////////////////////////////////////////////////////////////
        struct empty_view_t {
            using hana_tag = view_tag;
        };

        constexpr empty_view_t empty_view() {
            return {};
        }

        template <>
        struct is_view<empty_view_t> {
            static constexpr bool value = true;
        };

        //////////////////////////////////////////////////////////////////////
        // identity_view
        //////////////////////////////////////////////////////////////////////
        template <typename Xs>
        struct identity_view_t {
            Xs& sequence_;
            using hana_tag = view_tag;
        };

         template <typename Xs>
        constexpr identity_view_t<Xs> identity_view(Xs& xs) {
            return {xs};
        }

        template <typename Xs>
        struct is_view<identity_view_t<Xs>> {
            static constexpr bool value = true;
        };
    } // end namespace detail

    template <>
    struct make_impl<view_tag> {
        template <typename Xs>
        static constexpr detail::identity_view_t<Xs> apply(Xs& xs)
        { return {xs}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<hana::view_tag> {
        // cartesian_product_element_view
        template <typename Sequences, std::size_t i, typename F>
        static constexpr decltype(auto)
        apply(detail::cartesian_product_element_view_t<Sequences, i> view, F const& f) {
            using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
            return hana::unpack(view.sequences_,
                detail::make_cartesian_product_element_t<F, i>{f});
        }

        // cartesian_product_view
        template <typename View, typename F, std::size_t ...i>
        static constexpr decltype(auto)
        unpack_cartesian_product(View view, F&& f, std::index_sequence<i...>) {
          return static_cast<F&&>(f)(
              detail::cartesian_product_element_view_t<decltype(view.sequences_), i>{
                view.sequences_
              }...);
        }

        template <typename Sequences, typename F>
        static constexpr decltype(auto)
        apply(detail::cartesian_product_view_t<Sequences> view, F&& f) {
          using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
          return unpack_cartesian_product(view, static_cast<F&&>(f),
              std::make_index_sequence<Indices::length>{});
        }

        // sliced_view
        template <typename Sequence, std::size_t ...i, typename F>
        static constexpr decltype(auto)
        apply(detail::sliced_view_t<Sequence, i...> view, F&& f) {
            (void)view; // Remove spurious unused variable warning with GCC
            return static_cast<F&&>(f)(hana::at_c<i>(view.sequence_)...);
        }

        // transformed_view
        template <typename Sequence, typename F, typename G>
        static constexpr decltype(auto)
        apply(detail::transformed_view_t<Sequence, F> view, G&& g) {
            return hana::unpack(view.sequence_, hana::on(static_cast<G&&>(g), view.f_));
        }

        // joined_view
        template <typename View, typename F, std::size_t ...i1, std::size_t ...i2>
        static constexpr decltype(auto)
        unpack_joined(View view, F&& f, std::index_sequence<i1...>,
                                        std::index_sequence<i2...>)
        {
            (void)view; // Remove spurious unused variable warning with GCC
            return static_cast<F&&>(f)(hana::at_c<i1>(view.sequence1_)...,
                                       hana::at_c<i2>(view.sequence2_)...);
        }

        template <typename S1, typename S2, typename F>
        static constexpr decltype(auto)
        apply(detail::joined_view_t<S1, S2> view, F&& f) {
            constexpr auto N1 = decltype(hana::length(view.sequence1_))::value;
            constexpr auto N2 = decltype(hana::length(view.sequence2_))::value;
            return unpack_joined(view, static_cast<F&&>(f),
                                 std::make_index_sequence<N1>{},
                                 std::make_index_sequence<N2>{});
        }

        // flattened_view
        template <typename Sequences, typename F>
        static constexpr decltype(auto)
        apply(detail::flattened_view_t<Sequences> view, F&& f) {
            return detail::unpack_flatten(view.sequences_, static_cast<F&&>(f));
        }

        // single_view
        template <typename T, typename F>
        static constexpr decltype(auto) apply(detail::single_view_t<T> view, F&& f) {
            return static_cast<F&&>(f)(view.value_);
        }

        // empty_view
        template <typename F>
        static constexpr decltype(auto) apply(detail::empty_view_t, F&& f) {
            return static_cast<F&&>(f)();
        }

        // identity_view
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(detail::identity_view_t<Xs> view, F&& f) {
            return hana::unpack(view.sequence_, static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<hana::view_tag> {
        // cartesian_product_element_view
        template <typename Sequences, std::size_t i, typename N>
        static constexpr decltype(auto)
        apply(detail::cartesian_product_element_view_t<Sequences, i> view, N const&) {
            using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
            return hana::at_c<Indices::indices_of(i)[N::value]>(
                      hana::at_c<N::value>(view.sequences_));
        }

        // cartesian_product_view
        template <typename Sequences, typename N>
        static constexpr decltype(auto)
        apply(detail::cartesian_product_view_t<Sequences> view, N const&) {
          return detail::cartesian_product_element_view_t<Sequences, N::value>{view.sequences_};
        }

        // sliced_view
        template <typename Sequence, std::size_t ...i, typename N>
        static constexpr decltype(auto)
        apply(detail::sliced_view_t<Sequence, i...> view, N const&) {
            constexpr std::size_t indices[] = {i...};
            constexpr std::size_t n = indices[N::value];
            return hana::at_c<n>(view.sequence_);
        }

        // transformed_view
        template <typename Sequence, typename F, typename N>
        static constexpr decltype(auto)
        apply(detail::transformed_view_t<Sequence, F> view, N const& n) {
            return view.f_(hana::at(view.sequence_, n));
        }

        // joined_view
        template <std::size_t Left, typename View, typename N>
        static constexpr decltype(auto) at_joined_view(View view, N const&, hana::true_) {
            return hana::at_c<N::value>(view.sequence1_);
        }

        template <std::size_t Left, typename View, typename N>
        static constexpr decltype(auto) at_joined_view(View view, N const&, hana::false_) {
            return hana::at_c<N::value - Left>(view.sequence2_);
        }

        template <typename S1, typename S2, typename N>
        static constexpr decltype(auto)
        apply(detail::joined_view_t<S1, S2> view, N const& n) {
            constexpr auto Left = decltype(hana::length(view.sequence1_))::value;
            return at_joined_view<Left>(view, n, hana::bool_c<(N::value < Left)>);
        }

        // flattened_view
        struct index_pair { std::size_t sequence; std::size_t index; };
        template <std::size_t N, std::size_t ...Lenghts>
        struct flat_indexer {
            constexpr index_pair operator()() const {
                constexpr std::size_t sizes[] = {Lenghts...};
                std::size_t current = 0;
                std::size_t n = N;
                while (n >= sizes[current]) {
                    n -= sizes[current];
                    ++current;
                }
                return index_pair{current, n};
            }
        };
        template <std::size_t N>
        struct make_flat_indexer {
            template <typename ...S>
            constexpr auto operator()(S const& ...s) const
                -> flat_indexer<N, decltype(hana::length(s))::value...>
            { return {}; }
        };

        template <typename Sequences, typename N>
        static constexpr decltype(auto)
        apply(detail::flattened_view_t<Sequences> view, N const&) {
            using Indexer = decltype(hana::unpack(view.sequences_,
                                                  make_flat_indexer<N::value>{}));
            constexpr auto indices = Indexer{}();
            return hana::at_c<indices.index>(
                hana::at_c<indices.sequence>(view.sequences_)
            );
        }

        // single_view
        template <typename T, typename N>
        static constexpr decltype(auto) apply(detail::single_view_t<T> view, N const&) {
            static_assert(N::value == 0,
            "trying to fetch an out-of-bounds element in a hana::single_view");
            return view.value_;
        }

        // empty_view
        template <typename N>
        static constexpr decltype(auto) apply(detail::empty_view_t, N const&) = delete;

        // identity_view
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(detail::identity_view_t<Xs> view, N const& n) {
            return hana::at(view.sequence_, n);
        }
    };

    template <>
    struct length_impl<hana::view_tag> {
        // cartesian_product_view_element
        template <typename Sequences, std::size_t i>
        static constexpr auto apply(detail::cartesian_product_element_view_t<Sequences, i> view) {
            return hana::length(view.sequences_);
        }

        // cartesian_product_view
        template <typename Sequences>
        static constexpr auto apply(detail::cartesian_product_view_t<Sequences> view) {
            using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
            return hana::size_c<Indices::length>;
        }

        // sliced_view
        template <typename Sequence, std::size_t ...i>
        static constexpr auto
        apply(detail::sliced_view_t<Sequence, i...>) {
            return hana::size_c<sizeof...(i)>;
        }

        // transformed_view
        template <typename Sequence, typename F>
        static constexpr auto apply(detail::transformed_view_t<Sequence, F> view) {
            return hana::length(view.sequence_);
        }

        // joined_view
        template <typename S1, typename S2>
        static constexpr auto apply(detail::joined_view_t<S1, S2> view) {
            return hana::size_c<
                decltype(hana::length(view.sequence1_))::value +
                decltype(hana::length(view.sequence2_))::value
            >;
        }

        // flattened_view
        struct sum_lengths {
            template <typename ...S>
            constexpr auto operator()(S const& ...s) const {
                constexpr std::size_t lengths[sizeof...(S)] = {decltype(hana::length(s))::value...};
                constexpr std::size_t sum = detail::accumulate(lengths, lengths+sizeof...(S), 0);
                return hana::size_t<sum>{};
            }
        };

        template <typename Sequences>
        static constexpr auto apply(detail::flattened_view_t<Sequences> view) {
            return hana::unpack(view.sequences_, sum_lengths{});
        }

        // single_view
        template <typename T>
        static constexpr auto apply(detail::single_view_t<T> const&) {
            return hana::size_c<1>;
        }

        // empty_view
        static constexpr auto apply(detail::empty_view_t) {
            return hana::size_c<0>;
        }

        // identity_view
        template <typename Xs>
        static constexpr auto apply(detail::identity_view_t<Xs> const& view) {
            return hana::length(view.sequence_);
        }
    };

    template <>
    struct is_empty_impl<hana::view_tag> {
        // cartesian_product_view_element
        template <typename Sequences, std::size_t i>
        static constexpr auto apply(detail::cartesian_product_element_view_t<Sequences, i> view) {
            using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
            return hana::bool_c<Indices::length == 0>;
        }

        // cartesian_product_view
        template <typename Sequences>
        static constexpr auto apply(detail::cartesian_product_view_t<Sequences> view) {
            using Indices = decltype(detail::make_cartesian_product_indices(view.sequences_));
            return hana::bool_c<Indices::length == 0>;
        }

        // sliced_view
        template <typename Sequence, std::size_t ...i>
        static constexpr auto
        apply(detail::sliced_view_t<Sequence, i...>) {
            return hana::bool_c<sizeof...(i) == 0>;
        }

        // transformed_view
        template <typename Sequence, typename F>
        static constexpr auto apply(detail::transformed_view_t<Sequence, F> view) {
            return hana::is_empty(view.sequence_);
        }

        // joined_view
        template <typename S1, typename S2>
        static constexpr auto apply(detail::joined_view_t<S1, S2> view) {
            return hana::and_(hana::is_empty(view.sequence1_),
                              hana::is_empty(view.sequence2_));
        }

        // flattened_view
        template <typename Sequences>
        static constexpr auto apply(detail::flattened_view_t<Sequences> view) {
            return hana::all_of(view.sequences_, hana::is_empty);
        }

        // single_view
        template <typename T>
        static constexpr auto apply(detail::single_view_t<T> const&) {
            return hana::false_c;
        }

        // empty_view
        static constexpr auto apply(detail::empty_view_t) {
            return hana::true_c;
        }

        // identity_view
        template <typename Xs>
        static constexpr auto apply(detail::identity_view_t<Xs> const& view) {
            return hana::is_empty(view.sequence_);
        }
    };

    template <>
    struct drop_front_impl<hana::view_tag> {
        template <typename View, typename N>
        static constexpr auto apply(View view, N const&) {
            constexpr auto n = N::value;
            constexpr auto Length = decltype(hana::length(view))::value;
            return detail::sliced(view, hana::range_c<std::size_t, n, Length>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<hana::view_tag> {
        template <typename Sequence, typename F, typename G>
        static constexpr auto
        apply(detail::transformed_view_t<Sequence, F> view, G&& g) {
            return detail::transformed(view.sequence_,
                                       hana::compose(static_cast<G&&>(g), view.f_));
        }

        template <typename Sequence, typename F>
        static constexpr auto apply(detail::identity_view_t<Sequence> view, F&& f) {
            return detail::transformed(view.sequence_, static_cast<F&&>(f));
        }

        template <typename View, typename F>
        static constexpr auto apply(View view, F&& f) {
            return detail::transformed(view, static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<hana::view_tag> {
        template <typename T>
        static constexpr detail::single_view_t<typename std::decay<T>::type> apply(T&& t) {
            return {static_cast<T&&>(t)};
        }
    };

    template <>
    struct ap_impl<hana::view_tag> {
        template <typename F, typename X>
        static constexpr auto apply(F&& f, X&& x) {
            // TODO: Implement cleverly; we most likely need a cartesian_product
            //       view or something like that.
            return hana::ap(hana::to_tuple(f), hana::to_tuple(x));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<hana::view_tag> {
        template <typename Sequence>
        static constexpr auto apply(detail::identity_view_t<Sequence> view) {
            return detail::flattened(view.sequence_);
        }

        template <typename View>
        static constexpr auto apply(View view) {
            return detail::flattened(view);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<hana::view_tag> {
        template <typename View1, typename View2>
        static constexpr auto apply(View1 view1, View2 view2) {
            return detail::joined(view1, view2);
        }
    };

    template <>
    struct empty_impl<hana::view_tag> {
        static constexpr auto apply() {
            return detail::empty_view_t{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////

    template <typename Xs, typename Ys>
    inline constexpr auto equal_helper(Xs const& xs, Ys const& ys) {
        constexpr std::size_t xs_size = decltype(hana::length(xs))::value;
        constexpr std::size_t ys_size = decltype(hana::length(ys))::value;
        detail::compare_finite_sequences<Xs, Ys, xs_size> comp{xs, ys};
        return comp.template apply<0>(hana::bool_<xs_size == 0>{},
                                      hana::bool_<xs_size == ys_size>{});
    }

    template <>
    struct equal_impl<hana::view_tag, hana::view_tag> {
        template <typename View1, typename View2>
        static constexpr auto apply(View1&& v1, View2&& v2) {
            return equal_helper(static_cast<View1&&>(v1), static_cast<View2&&>(v2));
        }
    };

    template <typename S>
    struct equal_impl<hana::view_tag, S, hana::when<hana::Sequence<S>::value>> {
        template <typename View1, typename View2>
        static constexpr auto apply(View1&& v1, View2&& v2) {
            return equal_helper(static_cast<View1&&>(v1), static_cast<View2&&>(v2));
        }
    };

    template <typename S>
    struct equal_impl<S, hana::view_tag, hana::when<hana::Sequence<S>::value>> {
        template <typename View1, typename View2>
        static constexpr auto apply(View1&& v1, View2&& v2) {
            return equal_helper(static_cast<View1&&>(v1), static_cast<View2&&>(v2));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<hana::view_tag, hana::view_tag> {
        template <typename View1, typename View2>
        static constexpr auto apply(View1 v1, View2 v2) {
            return hana::lexicographical_compare(v1, v2);
        }
    };

    template <typename S>
    struct less_impl<hana::view_tag, S, hana::when<hana::Sequence<S>::value>> {
        template <typename View1, typename Seq>
        static constexpr auto apply(View1 v1, Seq const& s) {
            return hana::lexicographical_compare(v1, s);
        }
    };

    template <typename S>
    struct less_impl<S, hana::view_tag, hana::when<hana::Sequence<S>::value>> {
        template <typename Seq, typename View2>
        static constexpr auto apply(Seq const& s, View2 v2) {
            return hana::lexicographical_compare(s, v2);
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_VIEW_HPP
