/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <utility>
using namespace boost::hana;


// a non-movable, non-copyable type
struct RefOnly {
    RefOnly() = default;
    RefOnly(RefOnly const&) = delete;
    RefOnly(RefOnly&&) = delete;
};

struct Empty { };

int main() {
    // Make sure we can create a tuple of rvalue references
    {
        RefOnly e{};
        detail::closure<RefOnly&&> xs{std::move(e)};
        detail::closure<RefOnly&&, RefOnly&&> ys{std::move(e), std::move(e)};
        (void)xs; (void)ys;

        detail::closure<RefOnly&&> xs2 = {std::move(e)};
        detail::closure<RefOnly&&, RefOnly&&> ys2 = {std::move(e), std::move(e)};
        (void)xs2; (void)ys2;
    }

    // Make sure we can create a tuple of non-const lvalue references
    {
        RefOnly e{};
        detail::closure<RefOnly&> xs{e};
        detail::closure<RefOnly&, RefOnly&> ys{e, e};
        (void)xs; (void)ys;

        detail::closure<RefOnly&> xs2 = {e};
        detail::closure<RefOnly&, RefOnly&> ys2 = {e, e};
        (void)xs2; (void)ys2;
    }

    // Make sure we can create a tuple of const lvalue references
    {
        RefOnly const e{};
        detail::closure<RefOnly const&> xs{e};
        detail::closure<RefOnly const&, RefOnly const&> ys{e, e};
        (void)xs; (void)ys;

        detail::closure<RefOnly const&> xs2 = {e};
        detail::closure<RefOnly const&, RefOnly const&> ys2 = {e, e};
        (void)xs2; (void)ys2;
    }

    // Try to construct tuples with mixed references and non-reference members.
    {
        RefOnly r{};
        Empty e{};

        {
            detail::closure<RefOnly const&, Empty> xs{r, e};
            detail::closure<RefOnly const&, Empty> ys = {r, e};
            (void)xs; (void)ys;
        }
        {
            detail::closure<RefOnly&, Empty> xs{r, e};
            detail::closure<RefOnly&, Empty> ys = {r, e};
            (void)xs; (void)ys;
        }
        {
            detail::closure<RefOnly&&, Empty> xs{std::move(r), e};
            detail::closure<RefOnly&&, Empty> ys = {std::move(r), e};
            (void)xs; (void)ys;
        }
    }
}
