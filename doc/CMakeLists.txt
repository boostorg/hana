# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup the documentation
##############################################################################
if (NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen was not found; the 'doc' and the various "
                    "'gh-pages.*' targets will be unavailable.")
    return()
endif()

configure_file(Doxyfile.in Doxyfile @ONLY)
add_custom_target(doc
    COMMAND ! ${DOXYGEN_EXECUTABLE} Doxyfile 2>&1 | grep "warning"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

if (NOT GIT_FOUND)
    message(WARNING "Git was not found; the various 'gh-pages.*' targets will be unavailable.")
    return()
endif()

add_custom_target(gh-pages.clone
    COMMAND [ -e ${CMAKE_SOURCE_DIR}/doc/gh-pages ] || ${GIT_EXECUTABLE} clone ${BOOST_HANA_ORIGIN_URL} --depth 1 --branch=gh-pages doc/gh-pages
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Cloning the gh-pages branch into the doc/gh-pages directory"
    VERBATIM
)

add_custom_target(gh-pages.clean
    COMMAND ${CMAKE_COMMAND} -E remove *.png *.css *.js *.html
    COMMAND ${CMAKE_COMMAND} -E remove_directory search
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    DEPENDS gh-pages.clone
    COMMENT "Cleaning up doc/gh-pages"
    VERBATIM
)

add_custom_target(gh-pages.copy
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Copying the documentation from ${CMAKE_CURRENT_BINARY_DIR}/html to doc/gh-pages"
    DEPENDS doc gh-pages.clean
    VERBATIM
)

add_custom_target(gh-pages.update
    COMMAND ${GIT_EXECUTABLE} add --all .
    COMMAND ${GIT_EXECUTABLE} commit --allow-empty -m "Update to ${BOOST_HANA_CURRENT_COMMIT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Updating the gh-pages clone with freshly built documentation"
    DEPENDS gh-pages.copy
    VERBATIM
)

add_custom_target(gh-pages.push
    COMMAND ${GIT_EXECUTABLE} push origin gh-pages
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Pushing the gh-pages branch to GitHub"
    DEPENDS gh-pages.update
    VERBATIM
)
