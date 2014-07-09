<% render('techniques/foldl/functions.rb') %>

<% 0.upto(unroll).each do |n| %>
  <%= unroll_linear(n, trailing: n == unroll) %>
<% end %>

<%= render('techniques/foldl/main.cpp') %>
