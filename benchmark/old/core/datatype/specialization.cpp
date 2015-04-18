template <typename T, typename Enable = void*>
struct datatype_impl { using type = T; };

template <typename T>
struct datatype_impl<T, decltype((void*)(typename T::hana_datatype*)0)> {
    using type = typename T::hana_datatype;
};

template <typename T>
struct datatype {
    using type = typename datatype_impl<T>::type;
};

template <int i>
struct x { using hana_datatype = x; };

<% for i in 0..input_size %>
  template struct datatype<x<<%= i %>>>;
<% end %>
