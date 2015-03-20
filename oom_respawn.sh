#!/usr/bin/env bash

for job in 1 2 3 4 5 6; do
    if (travis show ${TRAVIS_BUILD_NUMBER}.${job} --repo ldionne/hana | grep --quiet -e "failed" -e "errored"); then
        if (travis logs ${TRAVIS_BUILD_NUMBER}.${job} --repo ldionne/hana --no-stream | grep --quiet -e "OOM" -e "Your test run exceeded 50 minutes"); then
            travis restart ${TRAVIS_BUILD_NUMBER}.${job} --repo ldionne/hana
        fi
    fi
done
