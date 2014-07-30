/*!
@file
Defines `boost::hana::datatype` and `boost::hana::datatype_t`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DATATYPE_HPP
#define BOOST_HANA_CORE_DATATYPE_HPP

namespace boost { namespace hana {
    namespace core_detail {
        template <typename T, typename Enable = void*>
        struct default_datatype { using type = T; };

        template <typename T>
        struct default_datatype<T, decltype((void*)(typename T::hana_datatype*)0)> {
            using type = typename T::hana_datatype;
        };
    }

    //! @ingroup group-core
    //! Metafunction returning the data type associated to `T`.
    //!
    //! By default, this metafunction returns `T::hana_datatype` if that
    //! expression is valid, and `T` otherwise. It can also be specialized
    //! to customize the data type of `T` without requiring `T` to have a
    //! nested `hana_datatype` type. A dummy parameter is also provided to
    //! allow `datatype` to be specialized for all types satisfying a predicate
    //! using `std::enable_if`.
    //!
    //! @todo
    //! - Could this be related to `decltype_`? If so, how? It is a valid
    //!   question whether `decltype_(list(...))` should be `List` or `<garbage>`.
    //! - Consider using two layers of specializations to improve performance
    //!   if this is an issue. I suspect that using the enabler will hurt
    //!   performance a lot because it requires the compiler to look at all
    //!   the enablers each time `datatype` is instantiated.
    template <typename T, typename Enable = void>
    struct datatype {
        using type = typename core_detail::default_datatype<T>::type;
    };

    //! @ingroup group-core
    //! Alias to `datatype<T>::%type`.
    template <typename T>
    using datatype_t = typename datatype<T>::type;
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DATATYPE_HPP
