#=============================================================================
# Setup the documentation
#=============================================================================
if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found; the 'doc' target will be unavailable.")
    return()
endif()

configure_file(Doxyfile.in Doxyfile @ONLY)
add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

if (NOT GIT_FOUND)
    message(STATUS "Git was not found; the 'gh-pages' target will be unavailable.")
    return()
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    OUTPUT_VARIABLE BOOST_HANA_GIT_SHORT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_custom_target(gh-pages
    COMMAND ${CMAKE_COMMAND} -E remove *.png *.css *.js *.html
    COMMAND ${CMAKE_COMMAND} -E remove_directory search
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMAND ${GIT_EXECUTABLE} add .
    COMMAND ${GIT_EXECUTABLE} commit -m "Update to ${BOOST_HANA_GIT_SHORT_SHA}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Updating the gh-pages branch with freshly built documentation"
    DEPENDS doc
    VERBATIM
)
