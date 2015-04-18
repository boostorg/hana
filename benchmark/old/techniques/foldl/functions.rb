<%
def unroll_linear(n, trailing: false)
  tparams = ['typename F', 'typename State', *(0...n).map { |i| "typename X#{i}" }]
  tparams << 'typename ...Xs' if trailing

  params = ['F f', 'State s', *(0...n).map { |i| "X#{i} x#{i}" }]
  params << 'Xs ...xs' if trailing

  args = (0...n).map { |i| "x#{i}" }
  foldl = args.foldl('s') { |s, x| "f(#{s}, #{x})" }
  foldl = "foldl(f, #{foldl}, xs...)" if trailing

  <<-EOS
  template <#{tparams.join(', ')}>
  constexpr auto foldl(#{params.join(', ')})
  { return #{foldl}; }
  EOS
end

def unroll_log(n)
  if n == 0
    unroll_linear(n, trailing: false)
  else
    unroll_linear(n, trailing: true) + "\n" + unroll_log(n/2)
  end
end
%>