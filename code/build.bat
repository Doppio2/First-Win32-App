@echo off

mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\3WG\code\win32_3wg.cpp user32.lib Gdi32.lib
popd
