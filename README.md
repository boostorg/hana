# Boost.Hana <a href="https://travis-ci.org/ldionne/hana" target="_blank">![Build Status][Travis.badge]</a> <a href="https://gitter.im/ldionne/hana" target="_blank">![Gitter Chat][Gitter.badge]</a> <a href="http://semver.org" target="_blank">![Version][semver.badge]</a>
> Your standard library for metaprogramming

## Disclaimers
This is not an official Boost library. However, a formal review will be asked
for shortly. The library is fairly stable, but no effort will be made to stay
backward compatible.


## Overview
<!-- Important: keep this in sync with example/overview.cpp -->
```cpp
#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <cassert>
#include <string>
using namespace boost::hana;
using namespace boost::hana::literals;

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

int main() {
    // Sequences capable of holding heterogeneous objects, and algorithms
    // to manipulate them.
    _tuple<Fish, Cat, Dog> animals{Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"}};
    _tuple<std::string,std::string,std::string> names = transform(animals, [](auto a) {
        return a.name;
    });
    assert(reverse(names) == make_tuple("Snoopy", "Garfield", "Nemo"));

    // No compile-time information is lost: even if `animals` can't be a
    // constant expression because it contains strings, its length is constexpr.
    static_assert(length(animals) == 3u, "");

    // Computations on types can be performed with the same syntax as that of
    // normal C++. Believe it or not, everything is done at compile-time.
    auto animal_types = make_tuple(type<Fish*>, type<Cat&>, type<Dog*>);
    auto animal_ptrs = filter(animal_types, [](auto a) {
        return traits::is_pointer(a);
    });
    static_assert(animal_ptrs == make_tuple(type<Fish*>, type<Dog*>), "");

    // And many other goodies to make your life easier, including:
    // 1. Access to elements in a tuple with a sane syntax.
    static_assert(animal_ptrs[0_c] == type<Fish*>, "");
    static_assert(animal_ptrs[1_c] == type<Dog*>, "");

    // 2. Unroll loops at compile-time without hassle.
    std::string s;
    int_<10>.times([&]{ s += "x"; });
    // equivalent to s += "x"; s += "x"; ... s += "x";

    // 3. Easily check whether an expression is valid.
    //    This is usually achieved with complex SFINAE-based tricks.
    auto has_name = is_valid([](auto&& x) -> decltype(x.name) { });
    static_assert(!has_name(type<int>), "");
    static_assert(has_name(type<Cat>), "");
}
```


## Prerequisites and installation
Boost.Hana is a header only library. To use it in your own project, just add
the [include](include) directory to your compiler's header search path and
you are done.

The library relies on a full-featured C++14 compiler and standard library,
but nothing else is required. As of April 2015, the only compiler known to
compile the full test suite is Clang >= 3.5.0 with a recent libc++. The code
is tested on OS X and Linux. Work on a GCC port is currently underway. The
goal is to support GCC 5 when it is released, and perhaps GCC 4.9 by providing
a version of Hana with reduced functionality.


## Documentation
You can browse the documentation online at http://ldionne.github.io/hana.
You can also get an offline version of the documentation by checking out
the `gh-pages` branch. To avoid overwriting the current directory, you
can clone the `gh-pages` branch into a subdirectory like `doc/gh-pages`:
```shell
git clone http://github.com/ldionne/hana --branch=gh-pages doc/gh-pages
```

After issuing this, `doc/gh-pages` will contain exactly the same static
website that's available online. Note that `doc/gh-pages` is automatically
ignored by git so updating the documentation won't pollute your index.


## Hacking on Hana
Setting yourself up to work on Boost.Hana is easy. First, you will need an
installation of [CMake][]. Once this is done, you can `cd` to the root of
the project and setup the build directory:
```shell
mkdir build
cd build
cmake ..
```

Usually, you'll want to specify a custom compiler because the system's
compiler is too old:
```shell
cmake .. -DCMAKE_CXX_COMPILER=/path/to/compiler
```

On Linux, you'll also need to specify a custom standard library (libc++).
You can set the location of your custom libc++ installation as follows:
```shell
cmake .. -DLIBCXX_ROOT=/path/to/libc++
```

You may also want to use a recent version of Boost if you want to use all the
adapters for Boost.MPL and Boost.Fusion. You can specify a custom path for
your Boost installation if you don't want the system-wide Boost to be used:
```shell
cmake .. -DBOOST_ROOT=/path/to/boost
```

You can now build and run the unit tests and the examples. Assuming you
used the Makefile generator with CMake, you can issue
```shell
make run.all
```

> #### Tip
> There is a Makefile at the root of the project which forwards everything
> to the `build` directory. This way, you can also issue those commands from
> the root of the project instead of the `build` directory.

You should be aware that compiling the unit tests is pretty time and RAM
consuming, especially the tests for external adapters. This is due to the
fact that Hana's unit tests are very thorough, and also that heterogeneous
sequences in other libraries tend to have horrible compile-time performance.

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
file is known to the build system. Let's suppose the relative path from the
root of the project to the new source file is `path/to/file.cpp`. When you
re-run the CMake generation step, two new targets will be created. The first
is a target named `compile.path.to.file`, which compiles that source file, and
the second is a target named `run.path.to.file`, which compiles and then runs
that source file.

> #### Tip for Sublime Text users
> If you use the provided [hana.sublime-project](hana.sublime-project) file,
> you can select the "[Hana] Build current file" build system. When viewing a
> file to which a target is associated (like a test or an example), you can
> then compile it by pressing ⌘B, or compile and then run it using ⇧⌘B.


## Project organization
The project is organized in a couple of subdirectories.
- The [benchmark](benchmark) directory contains compile-time and runtime
  benchmarks to make sure the library is as fast as advertised. The benchmark
  code is written mostly in the form of [eRuby][] templates. The templates
  are used to generate C++ files which are then compiled while gathering
  compilation and execution statistics.
- The [doc](doc) directory contains configuration files needed to generate
  the documentation. The `doc/gh-pages` subdirectory is automatically ignored
  by git; you can conveniently store a local copy of the documentation by
  cloning the gh-pages branch into that directory, as explained above.
- The [example](example) directory contains the source code for all the
  examples of both the tutorial and the reference documentation.
- The [include](include) directory contains the library itself, which is
  header only.
- The [test](test) directory contains the source code for all the unit tests.


## Contributing
Want to contribute? Great!

1. Fork it.
2. Create a branch (`git checkout -b feature/xxx`)
3. Do your modifications on that branch
4. Make sure you did not break anything (`make tests examples`)
5. Commit your changes (`git commit -am "Added feature xxx"`)
6. Push to the branch (`git push origin feature/xxx`)
7. Open a pull request


## Related material
I presented a [talk on Hana][Hana-CppCon] at [CppCon 2014][CppCon].
If you are interested in metaprogramming, you may also take a look at the
[MPL11][], which is how Hana started out, and [this talk][MPL11-C++Now] about
the MPL11 at [C++Now 2014][C++Now].

I also wrote my bachelor's thesis on Hana. It is available [here][Hana.thesis],
but unfortunately it is in french only. As part of this thesis, I presented a
formalization of metaprogramming using category theory, which can be found
[here][Hana.thesis.presentation].


## License
Please see [LICENSE.md](LICENSE.md).


<!-- Links -->
[C++Now]: http://cppnow.org
[CMake]: http://www.cmake.org
[CppCon]: http://cppcon.org
[Doxygen]: http://www.doxygen.org
[eRuby]: http://en.wikipedia.org/wiki/ERuby
[Gitter.badge]: https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg
[Hana-CppCon]: http://ldionne.github.io/hana-cppcon-2014
[Hana.thesis.presentation]: http://ldionne.github.io/hana-thesis
[Hana.thesis]: http://github.com/ldionne/hana-thesis
[MPL11-C++Now]: http://ldionne.github.io/mpl11-cppnow-2014
[MPL11]: http://github.com/ldionne/mpl11
[semver.badge]: https://img.shields.io/badge/semver-0.2.0-brightgreen.svg
[Travis.badge]: https://travis-ci.org/ldionne/hana.svg?branch=master
