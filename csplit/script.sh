#!/bin/bash

csplit -z -f 'temp' -b '%02d.txt' src.diff /diff/ {*}
