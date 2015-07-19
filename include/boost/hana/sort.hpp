/*!
@file
Defines `boost::hana::sort`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SORT_HPP
#define BOOST_HANA_SORT_HPP

#include <boost/hana/fwd/sort.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/array.hpp>
#include <boost/hana/detail/nested_by.hpp> // required by fwd decl
#include <boost/hana/length.hpp>
#include <boost/hana/less.hpp>

#include <cstddef>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename Predicate>
    constexpr auto sort_t::operator()(Xs&& xs, Predicate&& pred) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::sort(xs, predicate) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs),
                           static_cast<Predicate&&>(pred));
    }

    template <typename Xs>
    constexpr auto sort_t::operator()(Xs&& xs) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::sort(xs) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    namespace detail {
        template <std::size_t n, bool ...b>
        struct sorted_indices {
            static constexpr bool pred(std::size_t i, std::size_t j) {
                constexpr bool results[] = {b...};
                return results[i + n * j];
            }

            static constexpr auto sorted() {
                detail::array<std::size_t, n> indices{};
                detail::iota(&indices[0], &indices[n], 0);
                detail::sort(&indices[0], &indices[n], pred);
                return indices;
            }

            static constexpr auto value = sorted();
        };

        template <std::size_t n>
        constexpr auto cartesian_product2() {
            using Array = detail::array<std::size_t, n * n>;
            Array a{}, b{};
            for (std::size_t i = 0; i < n; ++i) {
                for (std::size_t j = 0; j < n; ++j) {
                    a[i + j * n] = i;
                    b[i + j * n] = j;
                }
            }
            return std::pair<Array, Array>{a, b};
        }
    }

    template <typename S, bool condition>
    struct sort_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred, std::size_t ...n, std::size_t ...nn>
        static constexpr auto helper(Xs&& xs, Pred&& pred, std::index_sequence<n...>,
                                                           std::index_sequence<nn...>)
        {
            constexpr auto prods = detail::cartesian_product2<sizeof...(n)>();

            using SortedIndices = detail::sorted_indices<
                sizeof...(n),
                static_cast<bool>(decltype(
                    pred(hana::at_c<prods.first[nn]>(static_cast<Xs&&>(xs)),
                         hana::at_c<prods.second[nn]>(static_cast<Xs&&>(xs)))
                )::value)...
            >;

            return hana::make<S>(
                hana::at_c<SortedIndices::value[n]>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs, typename Pred>
        static constexpr auto helper(Xs&&, Pred&&, std::index_sequence<>,
                                                   std::index_sequence<>)
        {
            return hana::make<S>();
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr std::size_t n = decltype(hana::length(xs))::value;
            return helper(static_cast<Xs&&>(xs), pred,
                          std::make_index_sequence<n>{},
                          std::make_index_sequence<n * n>{});
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return sort_impl::apply(static_cast<Xs&&>(xs), hana::less); }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SORT_HPP
