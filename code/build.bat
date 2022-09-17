@echo off

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build
cl -D_3WG_INTERNAL=1 -D_3WG_SLOW=1 -FC -Zi ..\3WG\code\win32_3wg.cpp user32.lib Gdi32.lib
popd
