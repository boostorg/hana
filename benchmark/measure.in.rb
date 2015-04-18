#!/usr/bin/env ruby
#
# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# When called as a program, this script runs the command line given in
# arguments and returns the total time. This is similar to the `time`
# command from Bash.
#
# This file can also be required as a Ruby module to gain access to the
# methods defined below.
#
# NOTE:
# This file must not be used as-is. It must be processed by CMake first.

require 'benchmark'
require 'open3'
require 'pathname'
require 'ruby-progressbar'
require 'tilt'


def time(template_relative, range, aspect)
  measure_file = Pathname.new("@CMAKE_CURRENT_SOURCE_DIR@/measure.cpp")
  template = Pathname.new(template_relative).expand_path("@CMAKE_CURRENT_SOURCE_DIR@")
  range = range.to_a

  make = -> (target) {
    command = "@CMAKE_COMMAND@ --build @CMAKE_BINARY_DIR@ --target #{target}"
    stdout, stderr, status = Open3.capture3(command)
  }

  progress = ProgressBar.create(format: '%p%% %t | %B |',
                                title: template_relative,
                                total: range.size,
                                output: STDERR)
  range.map do |n|
    # Evaluate the ERB template with the given environment, and save
    # the result in the `measure.cpp` file.
    code = Tilt::ERBTemplate.new(template).render(nil, input_size: n)
    measure_file.write(code)

    # Compile the file and get timing statistics. The timing statistics
    # are output to stdout when we compile the file because of the way
    # the `compile.benchmark.measure` CMake target is setup.
    stdout, stderr, status = make["compile.benchmark.measure"]
    raise "compilation error: #{stderr}\n\n#{code}" if not status.success?
    match = stdout.match(/\[compilation time: (.+)\]/i)

    # If we didn't match anything, that's because we went too fast, CMake
    # did not have the time to see the changes to the measure file and
    # the target was not rebuilt. So we sleep for a bit and then retry
    # this iteration.
    (sleep 0.2; redo) if match.nil?
    stat = match.captures[0].to_f if aspect == :compilation_time

    # Run the resulting program and get timing statistics. The statistics
    # should be written to stdout by the `measure` function of the
    # `measure.hpp` header.
    if aspect == :execution_time
      stdout, stderr, status = make["run.benchmark.measure"]
      raise "runtime error: #{stderr}\n\n#{code}" if not status.success?
      match = stdout.match(/\[execution time: (.+)\]/i)
      if match.nil?
        raise ("Could not find [execution time: ...] bit in the output. " +
               "Did you use the `measure` function in the `measure.hpp` header? " +
               "stdout follows:\n#{stdout}")
      end
      stat = match.captures[0].to_f
    end

    progress.increment
    [n, stat]
  end
ensure
  measure_file.write("")
  progress.finish if progress
end

def time_execution(erb_file, range)
  time(erb_file, range, :execution_time)
end

def time_compilation(erb_file, range)
  time(erb_file, range, :compilation_time)
end

if __FILE__ == $0
  command = ARGV.join(' ')
  time = Benchmark.realtime { `#{command}` }

  puts "[command line: #{command}]"
  puts "[compilation time: #{time}]"
end
