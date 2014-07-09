<% render(instance) %>
<%= includes(0..x) %>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/pair.hpp>
BOOST_HANA_CONSTEXPR_LAMBDA auto partition = [](auto pred, auto xs) {
    auto not_pred = [=](auto x) { return !pred(x); };
    return boost::hana::pair(
        boost::hana::filter(pred, xs),
        boost::hana::filter(not_pred, xs)
    );
};

<%= render('list/partition/main.cpp') %>
