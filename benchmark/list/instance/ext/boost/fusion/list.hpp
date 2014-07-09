#include <boost/hana/ext/boost/fusion.hpp>
#include <boost/fusion/container/generation/make_list.hpp>
auto list = [](auto ...xs) {
    return boost::fusion::make_list(xs...);
};