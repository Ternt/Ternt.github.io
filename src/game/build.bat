@echo off
cd /D "%~dp0"

if not exist build mkdir build

pushd build
emcc ../astro/main.c -o astro.js -g -Wall -I../astro -I../includes -L../libs -lraylib.web -w -DPLATFORM_WEB -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -std=c99 
popd build
