/*!
@file
Defines `boost::hana::find_if`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FIND_IF_HPP
#define BOOST_HANA_FIND_IF_HPP

#include <boost/hana/fwd/find_if.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/drop_while.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    struct Searchable; //! @todo include the forward declaration instead
    struct Sequence;
    struct Iterable;

    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto find_if_t::operator()(Xs&& xs, Pred&& pred) const {
        using S = typename datatype<Xs>::type;
        using FindIf = BOOST_HANA_DISPATCH_IF(find_if_impl<S>,
            _models<Searchable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Searchable, S>::value,
        "hana::find_if(xs, pred) requires 'xs' to be a Searchable");
    #endif

        return FindIf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
    }
    //! @endcond

    template <typename S, bool condition>
    struct find_if_impl<S, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    namespace detail {
        template <typename Xs, typename Pred, std::size_t i, std::size_t N, bool Done>
        struct advance_until;

        template <typename Xs, typename Pred, std::size_t i, std::size_t N>
        struct advance_until<Xs, Pred, i, N, false>
            : advance_until<Xs, Pred, i + 1, N, hana::value<decltype(
                std::declval<Pred>()(hana::at_c<i>(std::declval<Xs>()))
            )>()>
        { };

        template <typename Xs, typename Pred, std::size_t N>
        struct advance_until<Xs, Pred, N, N, false> {
            template <typename Ys>
            static constexpr auto apply(Ys&&) {
                return hana::nothing;
            }
        };

        template <typename Xs, typename Pred, std::size_t i, std::size_t N>
        struct advance_until<Xs, Pred, i, N, true> {
            template <typename Ys>
            static constexpr auto apply(Ys&& ys) {
                return hana::just(hana::at_c<i - 1>(static_cast<Ys&&>(ys)));
            }
        };
    }

    template <typename S>
    struct find_if_impl<S, when<_models<Sequence, S>::value>> {
        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&&) {
            constexpr std::size_t N = hana::value<decltype(hana::length(xs))>();
            return detail::advance_until<Xs&&, Pred&&, 0, N, false>::apply(
                static_cast<Xs&&>(xs)
            );
        }
    };

    template <typename It>
    struct find_if_impl<It, when<
        _models<Iterable, It>::value &&
        !_models<Sequence, It>::value
    >> {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::only_when(
                hana::compose(hana::not_, hana::is_empty),
                hana::front,
                hana::drop_while(static_cast<Xs&&>(xs),
                                 hana::compose(hana::not_, static_cast<Pred&&>(pred)))
            );
        }
    };

    template <typename T, std::size_t N>
    struct find_if_impl<T[N]> {
        template <typename Xs>
        static constexpr auto find_if_helper(Xs&&, decltype(hana::false_))
        { return hana::nothing; }

        template <typename Xs>
        static constexpr auto find_if_helper(Xs&& xs, decltype(hana::true_))
        { return hana::just(static_cast<Xs&&>(xs)[0]); }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            return find_if_helper(static_cast<Xs&&>(xs),
                hana::bool_<hana::value<decltype(pred(xs[0]))>()>
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FIND_IF_HPP
