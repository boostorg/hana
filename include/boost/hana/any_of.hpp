/*!
@file
Defines `boost::hana::any_of`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ANY_OF_HPP
#define BOOST_HANA_ANY_OF_HPP

#include <boost/hana/fwd/any_of.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/tail.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>


namespace boost { namespace hana {
    struct Sequence; //! @todo include the forward declaration instead
    struct Searchable;

    //! @cond
    template <typename Xs, typename Pred>
    constexpr auto any_of_t::operator()(Xs&& xs, Pred&& pred) const {
        using S = typename datatype<Xs>::type;
        using AnyOf = BOOST_HANA_DISPATCH_IF(any_of_impl<S>,
            _models<Searchable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Searchable, S>::value,
        "hana::any_of(xs, pred) requires 'xs' to be a Searchable");
    #endif

        return AnyOf::apply(static_cast<Xs&&>(xs), static_cast<Pred&&>(pred));
    }
    //! @endcond

    template <typename S, bool condition>
    struct any_of_impl<S, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    template <typename S>
    struct any_of_impl<S, when<_models<Sequence, S>::value>> {
        //! @cond
        template <std::size_t k, std::size_t Len>
        struct any_of_helper {
            template <typename Xs, typename Pred>
            static constexpr auto apply(bool prev_cond, Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::at_c<k>(xs)), hana::true_,
                                                               hana::false_);
                return prev_cond ? hana::true_
                    : any_of_impl::any_of_helper<k + 1, Len>::apply(cond,
                                        static_cast<Xs&&>(xs),
                                        static_cast<Pred&&>(pred));
            }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(hana::true_), Xs&&, Pred&&)
            { return hana::true_; }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(hana::false_), Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::at_c<k>(xs)), hana::true_,
                                                               hana::false_);
                return any_of_impl::any_of_helper<k + 1, Len>::apply(cond,
                                        static_cast<Xs&&>(xs),
                                        static_cast<Pred&&>(pred));
            }
        };

        template <std::size_t Len>
        struct any_of_helper<Len, Len> {
            template <typename Cond, typename Xs, typename Pred>
            static constexpr auto apply(Cond cond, Xs&&, Pred&&)
            { return cond; }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr std::size_t len = hana::value<
                decltype(hana::length(xs))
            >();
            return any_of_impl::any_of_helper<0, len>::apply(hana::false_,
                                            static_cast<Xs&&>(xs),
                                            static_cast<Pred&&>(pred));
        }
        //! @endcond
    };

    template <typename It>
    struct any_of_impl<It, when<
        _models<Iterable, It>::value &&
        !_models<Sequence, It>::value
    >> {
        template <typename Xs, typename Pred>
        static constexpr auto lazy_any_of_helper(decltype(hana::false_), bool prev_cond, Xs&& xs, Pred&& pred) {
            auto cond = hana::if_(pred(hana::front(xs)), hana::true_, hana::false_);
            decltype(auto) tail = hana::tail(static_cast<Xs&&>(xs));
            constexpr bool done = hana::value<decltype(hana::is_empty(tail))>();
            return prev_cond ? hana::true_
                : lazy_any_of_helper(hana::bool_<done>, cond,
                                     static_cast<decltype(tail)&&>(tail),
                                     static_cast<Pred&&>(pred));
        }

        template <typename Xs, typename Pred>
        static constexpr auto lazy_any_of_helper(decltype(hana::false_), decltype(hana::true_), Xs&&, Pred&&)
        { return hana::true_; }

        template <typename Xs, typename Pred>
        static constexpr auto lazy_any_of_helper(decltype(hana::false_), decltype(hana::false_), Xs&& xs, Pred&& pred) {
            auto cond = hana::if_(pred(hana::front(xs)), hana::true_, hana::false_);
            constexpr bool done = hana::value<decltype(
                hana::is_empty(hana::tail(xs))
            )>();
            return lazy_any_of_helper(hana::bool_<done>, cond,
                                      hana::tail(static_cast<Xs&&>(xs)),
                                      static_cast<Pred&&>(pred));
        }

        template <typename Cond, typename Xs, typename Pred>
        static constexpr auto lazy_any_of_helper(decltype(hana::true_), Cond cond, Xs&&, Pred&&)
        { return cond; }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr bool done = hana::value<decltype(hana::is_empty(xs))>();
            return lazy_any_of_helper(hana::bool_<done>, hana::false_,
                                      static_cast<Xs&&>(xs),
                                      static_cast<Pred&&>(pred));
        }
    };

    template <typename T, std::size_t N>
    struct any_of_impl<T[N]> {
        template <typename Xs, typename Pred>
        static constexpr bool any_of_helper(bool cond, Xs&& xs, Pred&& pred) {
            if (cond) return true;
            for (std::size_t i = 1; i < N; ++i)
                if (pred(static_cast<Xs&&>(xs)[i]))
                    return true;
            return false;
        }

        // Since an array contains homogeneous data, if the predicate returns
        // a compile-time logical at any index, it must do so at every index
        // (because the type of the elements won't change)! In this case, we
        // then only need to evaluate the predicate on the first element.
        template <typename Xs, typename Pred>
        static constexpr auto
        any_of_helper(decltype(hana::true_), Xs&& /*xs*/, Pred&&)
        { return hana::true_; }

        template <typename Xs, typename Pred>
        static constexpr auto
        any_of_helper(decltype(hana::false_), Xs&&, Pred&&)
        { return hana::false_; }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            auto cond = hana::if_(pred(static_cast<Xs&&>(xs)[0]), hana::true_,
                                                                  hana::false_);
            return any_of_helper(cond, static_cast<Xs&&>(xs),
                                       static_cast<Pred&&>(pred));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ANY_OF_HPP
