#!/bin/bash
cd "${0%/*}"

cd ../build
cmake ..
cmake --build .