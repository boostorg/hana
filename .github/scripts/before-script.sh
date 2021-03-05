#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

if [ "$JOB_UUID" = "b6589fc6ab" ] || [ "$JOB_UUID" = "356a192b79" ] || [ "$JOB_UUID" = "da4b9237ba" ] || [ "$JOB_UUID" = "77de68daec" ] || [ "$JOB_UUID" = "1b64538924" ] || [ "$JOB_UUID" = "ac3478d69a" ] || [ "$JOB_UUID" = "c1dfd96eea" ] || [ "$JOB_UUID" = "902ba3cda1" ] || [ "$JOB_UUID" = "fe5dbbcea5" ] || [ "$JOB_UUID" = "0ade7c2cf9" ] || [ "$JOB_UUID" = "b1d5781111" ] || [ "$JOB_UUID" = "17ba079149" ] || [ "$JOB_UUID" = "7b52009b64" ] || [ "$JOB_UUID" = "bd307a3ec3" ] || [ "$JOB_UUID" = "fa35e19212" ] || [ "$JOB_UUID" = "f1abd67035" ] || [ "$JOB_UUID" = "1574bddb75" ] || [ "$JOB_UUID" = "0716d9708d" ] || [ "$JOB_UUID" = "9e6a55b6b4" ] || [ "$JOB_UUID" = "b3f0c7f6bb" ] || [ "$JOB_UUID" = "91032ad7bb" ] || [ "$JOB_UUID" = "472b07b9fc" ] || [ "$JOB_UUID" = "12c6fc06c9" ] ; then
    if [[ "${BOOST_BUILD}" == "true" ]]; then
      rm -rf "${BOOST_DIR}/libs/hana"
      cp -rp ${TRAVIS_BUILD_DIR} "${BOOST_DIR}/libs/hana"
      export TRAVIS_BUILD_DIR="${BOOST_DIR}/libs/hana"
    fi

    git config --global user.name "Travis bot"
    git config --global user.email "<>"
    cd "${TRAVIS_BUILD_DIR}"
    (mkdir build && cd build && cmake .. ${CMAKE_OPTIONS})
fi

