@echo off

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build
cl -MT -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -D_3WG_INTERNAL=1 -D_3WG_SLOW=1 -FC -Z7 -Fmwin32_3wg.map ..\3WG\code\win32_3wg.cpp /link -opt:ref -subsystem:windows,5.02 user32.lib Gdi32.lib
popd
