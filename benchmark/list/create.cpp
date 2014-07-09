<% render(instance) %>
<%= includes(0..x) %>

template <int i> struct x { };

int main() {
    auto go =  <%= list(
        (0..x).map { |i| "x<#{i}>" },
        (0..x).map { |i| "x<#{i}>{}" }
    ) %>;
}
