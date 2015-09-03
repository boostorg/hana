# Copyright Louis Dionne 2015
# Copyright Markus J. Weber 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module checks whether the current compiler is supported, and
# provides friendly hints to the user.

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "3.5.0")
        message(WARNING "
    ### You appear to be using Clang ${CMAKE_CXX_COMPILER_VERSION}, which is known
    ### to be unable to compile Hana. Consider switching to
    ### Clang >= 3.5.0. If it is already installed on your
    ### system, you can tell CMake about it with
    ###
    ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
        ")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "6.1.0")
        message(WARNING "
    ### You appear to be using Apple's Clang ${CMAKE_CXX_COMPILER_VERSION}, which is
    ### shipped with Xcode < 6.3. Unfortunately, only Apple's Clang
    ### >= 6.1.0 (shipped with Xcode >= 6.3) is supported by Hana.
    ### You should consider updating to Xcode >= 6.3 (requires Yosemite)
    ### or using a non-Apple Clang >= 3.5.0, which can be installed via
    ### Homebrew with
    ###
    ###     brew install llvm --with-clang
    ###
    ### You can then tell CMake to use that non-system Clang with
    ###
    ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
        ")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    message(WARNING "
    ### You appear to be using GCC ${CMAKE_CXX_COMPILER_VERSION}, which is known
    ### to be unable to compile Hana. Support for GCC is on
    ### its way, but it's not there yet. Consider switching
    ### to Clang >= 3.5.0. If Clang is already installed on
    ### your system, you can tell CMake about it with
    ###
    ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
    ")
else()
    message(WARNING "
    ### You appear to be using a compiler that is not yet tested with Hana.
    ### Please tell us whether it successfully compiles or if and how it
    ### fails by opening an issue at https://github.com/ldionne/hana/issues.
    ### Thanks!
    ")
endif()
