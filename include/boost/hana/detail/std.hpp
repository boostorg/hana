/*!
 * @file
 * Redefines some standard components.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_STD_HPP
#define BOOST_HANA_DETAIL_STD_HPP

namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    //! Equivalent to `std::size_t`.
    using std_size_t = decltype(sizeof(int));
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_STD_HPP
