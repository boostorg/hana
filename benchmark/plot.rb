# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

require 'benchcc'


time_output, memusg_output, inputs = ARGV
inputs = inputs.split(';')
Benchcc::plot_time(time_output, *inputs)
Benchcc::plot_memusg(memusg_output, *inputs)