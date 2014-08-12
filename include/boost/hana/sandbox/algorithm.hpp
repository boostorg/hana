/*!
@file
Defines std-like `constexpr` algorithms.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SANDBOX_ALGORITHM_HPP
#define BOOST_HANA_SANDBOX_ALGORITHM_HPP

namespace boost { namespace hana { namespace sandbox {
    template <typename Compare, typename InputIterator1, typename InputIterator2>
    constexpr bool std_lexicographical_compare(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        Compare compare)
    {
        for (; first2 != last2; ++first1, ++first2) {
            if (first1 == last1 || compare(*first1, *first2))
                return true;
            else if (compare(*first2, *first1))
                return false;
        }
        return false;
    }

    template <typename BinaryPredicate, typename InputIterator1, typename InputIterator2>
    constexpr bool std_equal(
        InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        BinaryPredicate pred)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            if (!pred(*first1, *first2))
                return false;
        return first1 == last1 && first2 == last2;
    }
}}} // end namespace boost::hana::sandbox

#endif // !BOOST_HANA_SANDBOX_ALGORITHM_HPP
