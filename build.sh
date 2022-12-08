#!/bin/sh

if ! test -f "./build/nix/CMakeCache.txt"; then
    cmake -S . -B ./build/nix
fi

cmake --build ./build/nix
