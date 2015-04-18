template <typename T> typename T::hana_datatype datatype_impl(void*);
template <typename T> T datatype_impl(...);

template <typename T>
struct datatype {
    using type = decltype(datatype_impl<T>(nullptr));
};

template <int i>
struct x { using hana_datatype = x; };

<% for i in 0..input_size %>
  template struct datatype<x<<%= i %>>>;
<% end %>
