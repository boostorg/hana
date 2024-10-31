#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'


# Drone adding ruby
git clone https://github.com/sstephenson/rbenv.git ~/.rbenv
echo 'export PATH="$HOME/.rbenv/bin:$HOME/.rbenv/shims:$PATH"' >> ~/.bash_profile
exec $SHELL
pushd $HOME
git clone git@github.com:sstephenson/ruby-build.git
cd ruby-build
./install.sh
ruby-build 1.8.7-p352 ~/.rbenv/versions/1.8.7-p352
ruby-build 1.9.3-preview1 ~/.rbenv/versions/1.9.3-preview1
rbenv rehash
rbenv set-default 2.1.4
ruby --version
popd

DEPS_DIR="${HOME}/deps"
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}
if [[ "${COMPILER}" == "default" ]]; then COMPILER=clang++-7; fi
if [[ "${BOOST_VERSION}" == "default" ]]; then BOOST_VERSION=1.66.0; fi
if [[ "${COMPILER}" != "" ]]; then export CXX=${COMPILER}; fi
${CXX} --version
if [[ "${BOOST_VERSION}" != "" ]]; then
  BOOST_DIR=${DEPS_DIR}/boost-${BOOST_VERSION}
  if [[ "${BOOST_VERSION}" == "trunk" ]]; then
    BOOST_URL="http://github.com/boostorg/boost.git"
    git clone --depth 1 --recursive ${BOOST_URL} ${BOOST_DIR} || exit 1
    (cd ${BOOST_DIR} && ./bootstrap.sh && ./b2 headers) || exit 1
  else
    BOOST_URL="https://dl.bintray.com/boostorg/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION//\./_}.tar.gz"
    mkdir -p ${BOOST_DIR}
    { wget -O - ${BOOST_URL} | tar --strip-components=1 -xz -C ${BOOST_DIR}; } || exit 1
  fi
  # Make sure we don't conflict with the Hana shipped with Boost
  rm -r ${BOOST_DIR}/boost/{hana,hana.hpp} || exit 1

  CMAKE_OPTIONS+=" -DBOOST_ROOT=${BOOST_DIR}"
fi

if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
  CMAKE_URL="https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.tar.gz"
  mkdir cmake && wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
  export PATH=${DEPS_DIR}/cmake/bin:${PATH}
else
  true brew install cmake || true brew upgrade cmake
fi

cmake --version || true
if [[ "${BOOST_BUILD}" == "true" ]]; then
  (cd ${BOOST_DIR}/tools/build && ./bootstrap.sh && ./b2 install --prefix=${DEPS_DIR}/b2)
  export PATH=${DEPS_DIR}/b2/bin:${PATH}
  b2 --version || true # b2 --version returns 1
fi

if [[ "${CXX%%+*}" == "clang" ]]; then
    if [[ "${CXX}" == "clang++-3.9" ]]; then LLVM_VERSION="3.9.1";
  elif [[ "${CXX}" == "clang++-4.0" ]]; then LLVM_VERSION="4.0.1";
  elif [[ "${CXX}" == "clang++-5.0" ]]; then LLVM_VERSION="5.0.2";
  elif [[ "${CXX}" == "clang++-6.0" ]]; then LLVM_VERSION="6.0.1";
  elif [[ "${CXX}" == "clang++-7" ]];   then LLVM_VERSION="7.0.1";
  fi

  LLVM_URL="http://llvm.org/releases/${LLVM_VERSION}/llvm-${LLVM_VERSION}.src.tar.xz"
  LIBCXX_URL="http://llvm.org/releases/${LLVM_VERSION}/libcxx-${LLVM_VERSION}.src.tar.xz"
  LIBCXXABI_URL="http://llvm.org/releases/${LLVM_VERSION}/libcxxabi-${LLVM_VERSION}.src.tar.xz"

  mkdir -p llvm llvm/build llvm/projects/libcxx llvm/projects/libcxxabi
  wget -O - ${LLVM_URL} | tar --strip-components=1 -xJ -C llvm || exit 1
  wget -O - ${LIBCXX_URL} | tar --strip-components=1 -xJ -C llvm/projects/libcxx || exit 1
  wget -O - ${LIBCXXABI_URL} | tar --strip-components=1 -xJ -C llvm/projects/libcxxabi || exit 1
  (cd llvm/build && cmake .. -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}/llvm/install) || exit 1
  (cd llvm/build/projects/libcxx && make install -j2) || exit 1
  (cd llvm/build/projects/libcxxabi && make install -j2) || exit 1

  export CXXFLAGS="-isystem ${DEPS_DIR}/llvm/install/include/c++/v1"
  export LDFLAGS="-L ${DEPS_DIR}/llvm/install/lib -l c++ -l c++abi"
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DEPS_DIR}/llvm/install/lib"
fi

if [[ "${DOCUMENTATION}" == "true" ]]; then
  true brew install doxygen
  doxygen --version
fi

if [[ "${BENCHMARKS}" == "true" ]]; then
  rvm use 2.1 --install --binary --fuzzy
  gem install ruby-progressbar tilt
fi

echo '==================================> BEFORE_SCRIPT'

. $DRONE_BUILD_DIR/.drone/before-script.sh

echo '==================================> SCRIPT'

sources=($(find include doc test example -name "*.hpp" -or -name "*.cpp"))
LANG=POSIX grep '[^[:print:][:cntrl:]]' "${sources[@]}"; [[ $? == 1 ]] || exit 1
LANG=POSIX grep $'\r'                   "${sources[@]}"; [[ $? == 1 ]] || exit 1
LANG=POSIX grep $'\t'                   "${sources[@]}"; [[ $? == 1 ]] || exit 1
LANG=POSIX grep '[[:blank:]]$'          "${sources[@]}"; [[ $? == 1 ]] || exit 1
if [[ "${DOCUMENTATION}" == "true" ]]; then
  (cd build && ! make doc 2>&1 | grep -E "error") || exit 1

  if [[ "${TRAVIS_PULL_REQUEST}" == "false" && "${TRAVIS_BRANCH}" == "master" ]]; then
    # Suppress output to avoid leaking the token when the command fails
    git clone https://ldionne:${GITHUB_TOKEN}@github.com/boostorg/hana --depth 1 --branch=gh-pages doc/html &>/dev/null

    rm -rf doc/html/{search,*.png,*.css,*.js,*.html}
    cp -R build/doc/html/* doc/html/

    pushd doc/html
    git add --all .
    git commit --allow-empty -m "Update documentation to ${TRAVIS_COMMIT:0:7}"
    # Suppress output to avoid leaking the token
    git push origin gh-pages &>/dev/null
    popd
  fi
fi

if [[ "${BENCHMARKS}" == "true" ]]; then
  if [[ "${TRAVIS_PULL_REQUEST}" == "false" && "${TRAVIS_BRANCH}" == "master" ]]; then
    (cd build && make benchmarks) || exit 1
    compiler_slug=$(cd build && make travis_compiler_slug | grep 'travis_compiler_slug:' | cut -d ' ' -f 2)
    config_slug=$(cd build && make travis_config_slug | grep 'travis_config_slug:' | cut -d ' ' -f 2)

    # Suppress output to avoid leaking the token when the command fails
    git clone https://ldionne:${GITHUB_TOKEN}@github.com/boostorg/hana --depth 1 --branch=gh-pages doc/html &>/dev/null
    rm -rf doc/html/benchmarks/${config_slug}/${compiler_slug}/
    mkdir -p doc/html/benchmarks/${config_slug}/${compiler_slug}/
    for benchmark in $(ls build/benchmark/*.json | grep -v ".erb"); do
      cp ${benchmark} doc/html/benchmarks/${config_slug}/${compiler_slug}/
    done

    pushd doc/html
    git add --all .
    git commit --allow-empty -m "Update benchmarks to ${TRAVIS_COMMIT:0:7} for build type '${config_slug}' and compiler '${compiler_slug}'"
    # Suppress output to avoid leaking the token
    git push origin gh-pages &>/dev/null
    popd
  else
    export BOOST_HANA_JUST_CHECK_BENCHMARKS=true
    (cd build && make benchmarks -j2)
  fi
fi

if [[ "${UNIT_TESTS}" == "true" ]]; then
  (cd build && make tests examples -j2 -k) &&
  if [[ "${ENABLE_MEMCHECK}" == "true" ]]; then
    (cd build && ctest --output-on-failure -j2 -D ExperimentalMemCheck)
  else
    (cd build && ctest --output-on-failure -j2)
  fi
fi

if [[ "${BOOST_BUILD}" == "true" ]]; then
  # Build documentation
  (cd doc && b2) || exit 1
  if [[ ! -d doc/html ]]; then exit 1; fi

  # Build tests
  echo "using clang : : ${CXX} ;" > project-config.jam
  if [[ "${LDFLAGS}" == "" ]]; then
    (cd test && b2 toolset=clang cxxflags="-std=c++1y ${CXXFLAGS}" include="${BOOST_DIR}") || exit 1
  else
    (cd test && b2 toolset=clang cxxflags="-std=c++1y ${CXXFLAGS}" include="${BOOST_DIR}" linkflags="${LDFLAGS}") || exit 1
  fi
fi

fi
