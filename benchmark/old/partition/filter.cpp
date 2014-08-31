<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/pair.hpp>

BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto xs, auto pred) {
    return boost::hana::pair(
        boost::hana::filter(xs, pred),
        boost::hana::filter(xs, [=](auto x) { return boost::hana::not_(pred(x)); })
    );
};

<%= render('list/partition/main.cpp') %>
