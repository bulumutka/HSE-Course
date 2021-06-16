#! /usr/bin/env bash

 export CLANG_FORMAT_SUBMODULE='/Users/mishasdk/repo/HSE-Course/module-1/homework/Optional/clangformat-cmake'

 cmake -B build -S . &&\
     cmake --build build --target runner clangformat &&\
     ./build/runner
