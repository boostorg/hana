# Boost.Hana
> Heterogeneous combiNAtors for expressive metaprogramming


## Disclaimers
This is not an official Boost library. However, a formal review will be asked
for shortly. The library is unstable at the moment; do not use for production.


## Overview
<!-- Important: keep this in sync with example/overview.cpp -->
```cpp
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
#include <type_traits>
using namespace boost::hana;


struct President { std::string name; };
struct Car       { std::string name; };
struct City      { std::string name; };
auto name = [](auto x) { return x.name; };

int main() {
    // Heterogeneous sequences for value-level metaprogramming.
    auto stuff = list(President{"Obama"}, Car{"Toyota"}, City{"Quebec"});
    assert(reverse(fmap(name, stuff)) == list("Quebec", "Toyota", "Obama"));

    // Type-level metaprogramming works too.
    auto types = fmap(compose(metafunction<std::add_pointer>, decltype_), stuff);

    static_assert(std::is_same<
        decltype(head(types))::type, President*
    >{}, "");

    static_assert(std::is_same<
        decltype(at(int_<1>, types))::type, Car*
    >{}, "");

    static_assert(std::is_same<
        decltype(last(types))::type, City*
    >{}, "");

    // No compile-time information is lost (the assertion is done at compile-time).
    BOOST_HANA_CONSTANT_ASSERT(length(permutations(stuff)) == int_<3 * 2 * 1>);
}
```


## Prerequisites and installation
Boost.Hana is a header only library. To use it in your own project, just add
the [include](include) directory to your compiler's header search path and
you are done.

The library relies on a full-featured C++14 compiler and standard library, but
nothing else is required. As of July 2014, the only compiler known to compile
the test suite is Clang 3.5.0 (trunk). While Clang 3.4 has full support for
C++14, it has several C++14-related bugs that are fixed in 3.5 and make it
incapable of compiling Hana.


## Documentation
You can browse the documentation online at http://ldionne.github.io/hana.
You can also get an offline version of the documentation by checking out
the `doc/gh-pages` submodule at its latest version:
```shell
git submodule update --init --remote
```


## Hacking on Hana
Setting yourself up to work on Boost.Hana is easy. First, you will need an
installation of [CMake][]. Once this is done, you can `cd` to the root of
the project and setup the build directory:
```shell
mkdir build
cd build
cmake ..
```

You can now build and run the unit tests and the examples. I assume that you
used the Makefile generator with CMake; the commands may differ for other
generators:
```shell
make tests
make examples
```

> #### Tip
> There is a Makefile at the root of the project which forwards everything
> to the `build` directory. Hence, you can also issue those commands from the
> root of the project instead of the `build` directory.

There are also optional targets which are enabled only when the required
software is available on your computer. For example, generating the
documentation requires [Doxygen][] to be installed. An informative message
will be printed during the CMake generation step whenever an optional target
is disabled. You can install any missing software and then re-run the CMake
generation to update the list of available targets.

> #### Tip
> You can use the `help` target to get a list of all the available targets.

If you want to add unit tests or examples, just add a source file in `test/`
or `example/` and then re-run the CMake generation step so the new source
file is known to the build system. If the relative path from the root of
the  project to the new source file is `path/to/file.cpp`, a target named
`path.to.file` to compile the file will be created when CMake is run.

> #### Tip for Sublime Text users
> If you use the provided [hana.sublime-project](hana.sublime-project) file,
> you can select the "Build current file" build system to build the target
> associated to the current file.


## Project organization
The project is organized in a couple of subdirectories.
- The [benchmark](benchmark) directory contains compile-time benchmarks to
  make sure we're freakin' fast. The benchmark code is written mostly in the
  form of [eRuby][] templates. The templates are used to generate C++ files
  which are then compiled while gathering compilation statistics. The
  benchmarks are driven by CMake files. Note that currently the benchmarks
  will only work with Clang because of the [gem used][Benchcc] to drive the
  compiler and gather the statistics.
- The [doc](doc) directory contains configuration files needed to generate
  the documentation. Also, the [doc/gh-pages](doc/gh-pages) directory is
  a submodule tracking the `gh-pages` branch of the official repository at
  http://github.com/ldionne/hana, which contains the latest documentation.
- The [example](example) directory contains the source code for all the
  examples of both the tutorial and the reference documentation.
- The [include](include) directory contains the library itself, which is
  header only.
- The [test](test) directory contains the source code for all the unit tests.


## Contributing
Want to contribute? Great!

1. Fork it.
2. Create a branch (`git checkout -b feature_X`)
3. Do your modifications on that branch
4. Make sure you did not break anything (`make tests examples`)
5. Commit your changes (`git commit -am "Added feature X"`)
6. Push to the branch (`git push origin feature_X`)
7. Open a pull request


## License
Please see [LICENSE.md](LICENSE.md).


<!-- Links -->
[Benchcc]: http://github.com/ldionne/benchcc
[CMake]: http://www.cmake.org
[Doxygen]: http://www.doxygen.org
[eRuby]: http://en.wikipedia.org/wiki/ERuby
