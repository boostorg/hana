#include <boost/hana/ext/std/tuple.hpp>
#include <tuple>
auto list = [](auto ...xs) {
    return std::make_tuple(xs...);
};