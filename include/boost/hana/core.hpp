/*!
 * @file
 * Defines core utilities.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_HPP
#define BOOST_HANA_CORE_HPP

namespace boost { namespace hana {
    template <template <typename ...> class Typeclass>
    struct defaults { };

    template <typename T> typename T::hana_datatype datatype_impl(void*);
    template <typename T> T datatype_impl(...);

    template <typename T>
    struct datatype {
        using type = decltype(datatype_impl<T>(nullptr));
    };

    template <typename T>
    using datatype_t = typename datatype<T>::type;

    namespace operators {
        /*!
         * Use this as a dummy template parameter or base class to make
         * operators in the `boost::hana::operators` namespace ADL-findable
         * for a type.
         */
        struct enable { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_HPP
