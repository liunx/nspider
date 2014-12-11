#!/bin/bash - 

valgrind \
    -v \
    --tool=memcheck \
    --leak-check=full \
    --leak-resolution=high \
    --show-reachable=yes \
    --track-origins=yes \
    $1
