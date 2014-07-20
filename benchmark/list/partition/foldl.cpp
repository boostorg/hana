<% render(instance) %>
<%= includes((0..x).size) %>

#include <boost/hana/list/list.hpp>
using boost::hana::partition;

<%= render('list/partition/main.cpp') %>
