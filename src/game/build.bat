@echo off
cd /D "%~dp0"

if not exist build mkdir build

pushd build
if "1"=="1" (
  call cl ../astro/main.c /Femain.exe /nologo /FC /Z7 /Od /Ob1 /I../astro /I../includes /link /MANIFEST:EMBED /INCREMENTAL:NO /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd /LIBPATH:../libs gdi32.lib msvcrt.lib raylib.lib winmm.lib user32.lib shell32.lib
  REM call emcc ../astro/main.c -o astro.js -g -Wall -I../astro -I../includes -L../libs -lraylib.web -w -DPLATFORM_WEB -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -std=c99 
)
popd build
