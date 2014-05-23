breadth = (1...2).to_a
depth = (0...1000).step(5).to_a
breadth.product(depth).map { |breadth, depth|
  { breadth: breadth, depth: depth }
}