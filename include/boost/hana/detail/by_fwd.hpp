/*!
@file
Forward declares `boost::hana::detail::by`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_BY_FWD_HPP
#define BOOST_HANA_DETAIL_BY_FWD_HPP

namespace boost { namespace hana { namespace detail {
    template <typename Algorithm>
    struct by_t {
        template <typename Predicate, typename Object>
        constexpr decltype(auto)
        operator()(Predicate&& predicate, Object&& object) const;

        template <typename Predicate>
        constexpr decltype(auto) operator()(Predicate&& predicate) const;
    };

    template <typename Algorithm>
    struct by_holder { static constexpr by_t<Algorithm> by{}; };

    template <typename Algorithm>
    constexpr by_t<Algorithm> by_holder<Algorithm>::by;

    //! @ingroup group-details
    //! Provides a `.by` static constexpr function object.
    //!
    //! When creating a binary function object of type `Algo` whose signature
    //! is `Object x Predicate -> Return`, `by<Algo>` can be used as a base
    //! class to `Algo`. Doing so will provide a static constexpr member
    //! called `by`, which has the two following signatures:
    //! @code
    //!     Predicate x Object -> Return
    //!     Predicate -> (Object -> Return)
    //! @endcode
    //!
    //! In other words, `by` is a `curry`ed and `flip`ped version of `Algo`.
    //! Note that the function object `Algo` must be default-constructible,
    //! since the algorithm will be called as `Algo{}(arguments...)` by `by`.
    //!
    //! @note
    //! This function object is especially useful because it takes care of
    //! avoiding ODR violations caused by the nested static constexpr member.
    template <typename Algorithm>
    using by = by_holder<Algorithm>;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_BY_FWD_HPP
