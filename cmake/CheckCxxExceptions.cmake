# Copyright Louis Dionne 2015
# with contributions of Markus J. Weber 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module checks whether the compiler and linker supports C++
# Exception Handling. This should catch two possible issues:
#   1) The user provided a compiler or linker flag to deactivate exceptions,
#      such as -fno-exceptions for GCC/Clang/Intel C++
#   2) The linker fails due to some internal error with exceptions,
#      which is at least a known issue for some Cygwin configurations using
#      Clang.
#
# Approach: First the existence of the macro symbol __EXCEPTIONS is checked,
# which is set if exceptions should be available. If available, then a
# compilation check is pursued. If that fails, a message is produced and
# exceptions are deactivated for the further process.
#
# If ultimately exceptions work, then the cmake variable is set:
#    CXX_EXCEPTIONS_ENABLED = 1
# if they are deactivated or didn't compile
#    CXX_EXCEPTIONS_ENABLED = 0
#
# Furthermore, the compiler flag '-fno-exceptions' is automatically added if
# the compilation check fails. Unfortunately a re-check is not performed since
# CMake caches the test results.

# ---------------------------------------------------------------------------

# Check whether the compiler should support exceptions
include(CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS(__EXCEPTIONS "" CXX_EXCEPTIONS_ENABLED)

if(NOT CXX_EXCEPTIONS_ENABLED)
    message("-- C++ exceptions in compiler: DISABLED")
else()
    message("-- C++ exceptions in compiler: ENABLED")

    # If exceptions are enabled, then the following code should compile and run
    # successfully:
    include(CheckCXXSourceRuns)
    check_cxx_source_runs("
        #include <iostream>
        int dummy( int a ) { if (a <0) throw a; return a; };
        int main () {
            int a = 0;
            try { a = dummy( -1 ); }
            catch (int e) { std::cout << e << std::endl; }
            std::cout << a << std::endl;
            return 0;
        }
        " CXX_EXCEPTIONS_COMPILE)

    # If exceptions are enabled, but don't compile, then try to disable them.
    if (NOT CXX_EXCEPTIONS_COMPILE)

        # Actually change the compiler flags
        set(CMAKE_CXX_FLAGS "-fno-exceptions ${CMAKE_CXX_FLAGS}" )

        message("
    ### WARNING: Your C++ Compiler failed at compiling with exceptions.
    ###
    ### Boost::Hana is not depending on C++ exceptions, but some examples
    ### might and perhaps your own code does as well.
    ###
    ### The build process will now proceed without exceptions by adding
    ### the compiler flag '-fno-exceptions'. This should be sufficient
    ### for most compilers for the present tests of Boost::Hana.
    ")
        if(CYGWIN AND (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
        message(
"    ### However, since you're running Cygwin, you might see a known problem
    ### with Clang on Windows. You should then add the C++ compiler flag
    ### '-fno-exceptions' also manually to your own code.
    ")
        endif()

        # It seems we cannot simply repeat the check, thus we need to disable
        # the exceptions manually...
        set(CXX_EXCEPTIONS_ENABLED 0)
        message("-- CXX compiler flag '-fno-exceptions' added.")
    endif()
    # Remove the temporary variables of the compilation check.
    unset(CXX_EXCEPTIONS_COMPILE)
    unset(CXX_EXCEPTIONS_COMPILE_COMPILED)
endif()
