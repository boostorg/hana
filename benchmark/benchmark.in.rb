# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

require 'benchcc'
require 'pathname'


CMAKE_CXX_COMPILER = Pathname.new("@CMAKE_CXX_COMPILER@").expand_path
PROJECT_SOURCE_DIR = Pathname.new("@PROJECT_SOURCE_DIR@").expand_path

environments_file = Pathname.new(ARGV[0]).expand_path
environments = TOPLEVEL_BINDING.eval(environments_file.read)
output_file = Pathname.new(ARGV[1]).expand_path
input_file = Pathname.new(ARGV[2]).expand_path
compiler = Benchcc::Compiler.guess_from_binary(CMAKE_CXX_COMPILER)


benchmark_opts = {
  timeout: 30,
  relative_to: PROJECT_SOURCE_DIR + 'benchmark',
  progressbar_format: '%p%% | %B |'
}

compiler_opts = [
  '-std=c++1y',
  '-fsyntax-only',
  "-I#{PROJECT_SOURCE_DIR + 'include'}",
  "-I#{PROJECT_SOURCE_DIR + 'benchmark'}"
]

data = Benchcc::benchmark(input_file, environments, **benchmark_opts) do |code|
  compiler.compile_code(code, *compiler_opts)
end.to_csv

output_file.dirname.mkpath
output_file.write(data)