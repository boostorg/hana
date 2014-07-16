<% render(instance) %>
<%= includes(x) %>

#include <boost/hana/list/list.hpp>
using boost::hana::partition;

<%= render('list/partition/main.cpp') %>
