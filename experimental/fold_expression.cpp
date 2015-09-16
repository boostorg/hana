/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>

#include <utility>
namespace hana = boost::hana;


#ifdef BOOST_HANA_CONFIG_HAS_CXX1Z_FOLD_EXPRESSIONS
// Experiment with fold expressions; see N4191 at
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4191.html
template <typename F, typename State>
struct accumulator {
    F f;
    State state;

    template <typename X>
    constexpr decltype(auto) operator+(X&& x) const& {
        return hana::detail::create< ::accumulator>{}(
            f, f(state, static_cast<X&&>(x))
        );
    }

    template <typename X>
    constexpr decltype(auto) operator+(X&& x) & {
        return hana::detail::create< ::accumulator>{}(
            f, f(state, static_cast<X&&>(x))
        );
    }

    template <typename X>
    constexpr decltype(auto) operator+(X&& x) && {
        decltype(auto) result = f(std::move(state),
                                  static_cast<X&&>(x));
        return hana::detail::create< ::accumulator>{}(
            std::move(f),
            static_cast<decltype(result)&&>(result)
        );
    }

    constexpr State const& get() const& { return state; }
    constexpr State & get() & { return state; }
    constexpr State get() && { return std::move(state); }
};

struct foldl_t {
    template <typename ...Xs>
    static constexpr decltype(auto) helper(Xs&& ...xs)
    { return (... + static_cast<Xs&&>(xs)); }

    template <typename F, typename State, typename ...Xs>
    constexpr decltype(auto) operator()(F&& f, State&& state, Xs&& ...xs) const {
        return helper(
            hana::detail::create<accumulator>{}(
                static_cast<F&&>(f),
                static_cast<State&&>(state)
            ),
            static_cast<Xs&&>(xs)...
        ).get();
    }
};

constexpr foldl_t foldl{};
#endif

int main() {

}
