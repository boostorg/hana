/*!
 * @file
 * Defines `boost::hana::Iterable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_HPP

namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Iterable;

    constexpr struct _head {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::head_impl(iterable); }
    } head{};

    constexpr struct _tail {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::tail_impl(iterable); }
    } tail{};

    constexpr struct _is_empty {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::is_empty_impl(iterable); }
    } is_empty{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
