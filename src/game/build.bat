@echo off
cd /D "%~dp0"

:: --- Unpack Arguments -------------------------------------------------------
for %%a in (%*) do set "%%~a=1"
if not "%msvc%"=="1" if not "%clang%"=="1" set msvc=1
if not "%release%"=="1" set debug=1
if "%debug%"=="1" set release=0 && echo [release mode]
if "%release%"=="1" set debug=0 && echo [debug mode]
if "%msvc%"=="1" set clang=0 && echo [msvc compile]
if "%clang%"=="1" set msvc=0 && echo [clang compile]

:: --- Unpack Command Line Build Arguments ------------------------------------
set auto_compile_flags=
if "%tracy%"=="1" set auto_compile_flags=%auto_compile_flags% -DPROFILE_TRACY=1

:: --- Compile/Link Line Definitions ------------------------------------------
if "%msvc%"=="1"   set cl_common=     /nologo /I../astro /I../includes %auto_compile_flags%
if "%msvc%"=="1"   set cl_debug=      call cl %cl_common% /Z7 /Od /DBUILD_DEBUG=1
if "%msvc%"=="1"   set cl_release=    call cl %cl_common% /Z7 /O2 /DBUILD_DEBUG=0
if "%msvc%"=="1"   set cl_link=       /link /MANIFEST:EMBED /INCREMENTAL:NO /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd /LIBPATH:../libs 
if "%msvc%"=="1"   set cl_out=        /Fe:
if "%clang%"=="1"  set clang_common=   
if "%clang%"=="1"  set clang_debug=    
if "%clang%"=="1"  set clang_release=  
if "%clang%"=="1"  set clang_link=     
if "%clang%"=="1"  set clang_out=      
if "%emcc%"=="1"   set emcc_common=   -Wall /I../astro /I../includes %auto_compile_flags%
if "%emcc%"=="1"   set emcc_debug=    call emcc %emcc_common% -g -w -DPLATFORM_WEB -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -std=c99
if "%emcc%"=="1"   set emcc_release=  call emcc %emcc_common% -w -DPLATFORM_WEB -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -std=c99
if "%emcc%"=="1"   set emcc_link=     -L../libs 
if "%emcc%"=="1"   set emcc_out=      -o 

:: --- Per-Build Configuration ------------------------------------------------
if "%msvc%"=="1"    set only_compile=/c
if "%clang%"=="1"   set only_compile=
if "%msvc%"=="1"    set EHsc=/EHsc
if "%clang%"=="1"   set EHsc=
if "%msvc%"=="1"    set link_dll=/link /DLL
if "%clang%"=="1"   set link_dll=
if "%msvc%"=="1"    set nostdlib=/NODEFAULTLIB
if "%clang%"=="1"   set nostdlib=-nostdlib

:: --- Choose Compile/Link Lines ----------------------------------------------
if "%msvc%"=="1"     set compile_debug=%cl_debug%
if "%msvc%"=="1"     set compile_release=%cl_release%
if "%msvc%"=="1"     set compile_link=%cl_link%
if "%msvc%"=="1"     set out=%cl_out%
if "%clang%"=="1"    set compile_debug=%clang_debug%
if "%clang%"=="1"    set compile_release=%clang_release%
if "%clang%"=="1"    set compile_link=%clang_link%
if "%clang%"=="1"    set out=%clang_out%
if "%debug%"=="1"    set compile=%compile_debug%
if "%release%"=="1"  set compile=%compile_release%

:: --- Prepare Build Directory -----------------------------------------------
if not exist build mkdir build

:: --- Build -----------------------------------------------------------------
pushd build
if "1"=="1" (
  %compile% ../test/test_custom_renderer.c %compile_link% gdi32.lib msvcrt.lib raylib.lib winmm.lib user32.lib shell32.lib %out%test_custom_renderer.exe 
  call emcc ../test/test_custom_renderer.c -o astro.js -g -Wall -I../astro -I../includes -L../libs -lraylib.web -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -w -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES3 -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -std=c99 --preload-file ../astro/fonts
)
popd build
