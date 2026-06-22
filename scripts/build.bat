@echo off
cd %~dp0
setlocal

pushd ..\src\games
for /d %%d in (*) do (
  if exist "%%d\build.bat" (
    echo Compiling %%d
    pushd "%%d"
    call build.bat
    popd
  )
)
popd
