/*!
@file
Defines `boost::hana::cartesian_product`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CARTESIAN_PRODUCT_HPP
#define BOOST_HANA_CARTESIAN_PRODUCT_HPP

#include <boost/hana/fwd/cartesian_product.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/array.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/unpack.hpp>

#include <cstddef>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs>
    constexpr auto cartesian_product_t::operator()(Xs&& xs) const {
        using S = typename hana::tag_of<Xs>::type;
        using CartesianProduct = BOOST_HANA_DISPATCH_IF(
            cartesian_product_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::cartesian_product(xs) requires 'xs' to be a Sequence");
    #endif

        return CartesianProduct::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    namespace detail {
        template <std::size_t ...Lengths>
        struct cartesian_product_indices {
            static constexpr std::size_t total_length() {
                std::size_t lengths[sizeof...(Lengths)] = {Lengths...};
                std::size_t r = 1;
                for (std::size_t len: lengths)
                    r *= len;
                return r;
            }

            static constexpr std::size_t length = total_length();

            static constexpr auto indices_of(std::size_t i) {
                constexpr std::size_t n = sizeof...(Lengths);
                constexpr std::size_t lengths[n] = {Lengths...};
                detail::array<std::size_t, n> result{};
                if (length == 0) {
                    return result;
                }
                for (std::size_t j = n; j--;) {
                    result[j] = i % lengths[j];
                    i /= lengths[j];
                }
                return result;
            }

            template <std::size_t n, typename F, std::size_t ...k, typename ...Xs>
            static constexpr auto
            product_element(std::index_sequence<k...>, F&& f, Xs&& ...xs) {
                constexpr auto indices = indices_of(n);
                return static_cast<F&&>(f)(hana::at_c<indices[k]>(xs)...);
            }

            template <std::size_t ...n, typename F, typename ...Xs>
            static constexpr auto
            create_product(std::index_sequence<n...>, F&& f, Xs&& ...xs) {
                return F{}(product_element<n>(
                    std::make_index_sequence<sizeof...(Xs)>{},
                    static_cast<F&&>(f), xs...
                )...);
            }
        };

        template <>
        struct cartesian_product_indices<> {
            static constexpr std::size_t total_length()
            { return 0; }

            static constexpr std::size_t length = 0;
        };

        struct make_cartesian_product_indices_helper_t {
            template <typename ...Xs>
            constexpr auto operator()(Xs&& ...xs) const
                -> detail::cartesian_product_indices<
                       decltype(hana::length(xs))::value...
                   >
            { return {}; }
        };

        struct make_cartesian_product_indices_t {
            template <typename Xs>
            constexpr auto operator()(Xs&& xs) const {
                return hana::unpack(
                    static_cast<Xs&&>(xs),
                    make_cartesian_product_indices_helper_t{});
            }
        };

        constexpr make_cartesian_product_indices_t make_cartesian_product_indices{};

        template <typename F, std::size_t n>
        struct make_cartesian_product_element_t {
            F const& f;

            constexpr auto operator()() const {
                return f();
            }

            template <typename X1, typename ...Xs>
            constexpr auto operator()(X1&& x1, Xs&& ...xs) const {
                constexpr auto indices = detail::cartesian_product_indices<
                    decltype(hana::length(x1))::value,
                    decltype(hana::length(xs))::value...
                >{};
                return indices.template product_element<n>(
                    std::make_index_sequence<sizeof...(Xs) + 1>{}, f,
                    static_cast<X1&&>(x1),
                    static_cast<Xs&&>(xs)...
                );
            }
        };
    }

    // Credits: implementation adapted from http://github.com/alexk7/hel.
    template <typename S, bool condition>
    struct cartesian_product_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            return hana::unpack(static_cast<Xs&&>(xs), cartesian_product_impl{});
        }

        template <typename ...Xs>
        constexpr auto operator()(Xs&& ...xs) const {
            using indices = detail::cartesian_product_indices<
                decltype(hana::length(xs))::value...
            >;
            return indices::template create_product(
                        std::make_index_sequence<indices::length>{},
                        hana::make<S>,
                        static_cast<Xs&&>(xs)...);
        }

        constexpr auto operator()() const {
            return hana::make<S>();
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_CARTESIAN_PRODUCT_HPP
