# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module checks the compiler and linker for compatibility and
# provides hints to the user.

if    (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
   if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "3.5.0")
      message(WARNING "
      ###
      ###   You appear to use Clang++ compiler version ${CMAKE_CXX_COMPILER_VERSION},
      ###   which is most likely unable to compile Boost::Hana.
      ###
      ")
      SET(SHOW_PREFERRED_COMPILER ON)
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    SET(SHOW_PREFERRED_COMPILER ON)
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "5.1.0")
      message("
      ###
      ###   You appear to use GNU C++ compiler version ${CMAKE_CXX_COMPILER_VERSION},
      ###   which is known to be unable to compile Boost::Hana.
      ###
      ###   You should at least upgrade to version >= 5.1.0
      ###
      ")
    else()
      message("
      ###
      ###   You appear to use GNU C++ compiler version ${CMAKE_CXX_COMPILER_VERSION}.
      ###
      ###   The compatibility with Boost::Hana is not yet confirmed.
      ###
      ")
    endif()
else()
      message("
      ###
      ###   NOTE: You appear to use a compiler that is not yet tested
      ###         with Boost::Hana. Please tell us about whether it
      ###         successfully compiles or if and how it failes.
      ###
      ###         Thanks !
      ###
      ")
      SET(SHOW_PREFERRED_COMPILER ON)
endif()

if(SHOW_PREFERRED_COMPILER)
    message("
      ###
      ###   ---   Consider switching to Clang >= 3.5.0   ---
      ###
      ###   If Clang is already installed on your system, then tell
      ###   CMake about it by using
      ###
      ###     cmake -DCMAKE_CXX_COMPILER=clang++ ...
      ###
    ")
endif()

