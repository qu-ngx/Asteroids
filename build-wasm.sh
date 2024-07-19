#!/bin/bash
set -euo pipefail

cd ../emsdk
source emsdk_env.sh
cd ../Asteroids-deploy
cd src

emcc -o ../out/asteroids.html \
    main_old.cpp -Os -Wall /Users/quangnguyen/Downloads/raylib-5.0/src/web/libraylib.a \
    -I. -I /Users/quangnguyen/Downloads/raylib-5.0/src/external \
    -L. -L /Users/quangnguyen/Downloads/raylib-5.0 \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    --shell-file ../shell.html \
    -s TOTAL_MEMORY=1073741824 \
    -s TOTAL_STACK=1024MB \
    -s INITIAL_MEMORY=2024MB \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s NO_EXIT_RUNTIME=1 \
    -s ASSERTIONS \
    -DPLATFORM_WEB

cd ../out
# BUILD_NAME="$(date -u +"%Y-%m-%d")"
# zip "${BUILD_NAME}.zip" index.html index.js index.wasm index.data

emrun asteroids.html
