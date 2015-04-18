template <typename ...>
struct result { };

constexpr struct {
    template <typename State, typename X>
    constexpr result<State, X> operator()(State, X) const { return {}; }
} f{};

constexpr struct { } state{};

template <int> struct x { };


int main() {
    auto go = foldl(
        <%= (['f', 'state'] + (0...input_size).map { |n| "x<#{n}>{}" }).join(', ') %>
    );
}
