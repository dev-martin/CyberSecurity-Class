#! /bin/bash

docker run --privileged --rm -v "$(pwd):/opt" -w /opt baseline ./constrained_encrypt2 $@

