#=============================================================================
# Setup the documentation
#=============================================================================
if (DOXYGEN_FOUND)
    configure_file(Doxyfile.in Doxyfile @ONLY)
    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE} Doxyfile
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else()
    message(STATUS "Doxygen not found; the 'doc' target will be unavailable.")
endif()
