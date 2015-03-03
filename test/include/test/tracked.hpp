/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_TRACKED_HPP
#define BOOST_HANA_TEST_TEST_TRACKED_HPP

#include <boost/hana/assert.hpp>

#include <iostream>


namespace boost { namespace hana { namespace test {
    struct Tracked {
        enum class State { CONSTRUCTED, MOVED_FROM, DESTROYED };

        int value;
        State state;

        explicit Tracked(int k) : value{k}, state{State::CONSTRUCTED} {
            std::cerr << "constructing " << *this << '\n';
        }

        Tracked(Tracked const& t) : value{t.value}, state{State::CONSTRUCTED} {
            BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                "copying a moved-from object");

            BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                "copying a destroyed object");

            std::cerr << "copying " << *this << '\n';
        }

        Tracked(Tracked&& t) : value{t.value}, state{State::CONSTRUCTED} {
            BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                "double moving from an object");

            BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                "moving from a destroyed object");

            std::cerr << "moving " << t << '\n';
            t.state = State::MOVED_FROM;
        }

        ~Tracked() {
            BOOST_HANA_RUNTIME_CHECK(state != State::DESTROYED &&
                "double-destroying an object");

            std::cerr << "destructing " << *this << '\n';
            state = State::DESTROYED;
        }

        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>&
        operator<<(std::basic_ostream<CharT, Traits>& os, Tracked const& t) {
            os << "Tracked{" << t.value << "}";
            switch (t.state) {
            case State::CONSTRUCTED:
                os << "[ok]"; break;
            case State::MOVED_FROM:
                os << "[moved from]"; break;
            case State::DESTROYED:
                os << "[destroyed]"; break;
            default:
                BOOST_HANA_RUNTIME_CHECK(false && "never reached");
            }
            return os;
        }
    };
}}}

#endif // !BOOST_HANA_TEST_TEST_TRACKED_HPP
