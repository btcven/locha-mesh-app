#!/bin/bash
docker run --rm --name test -v $PWD:/builds/btcven/locha/app -ti ubuntu:18.04 bash
