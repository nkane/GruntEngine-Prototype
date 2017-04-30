:: Go up one directory from code
popd

IF NOT EXIST .\build mkdir .\build

pushd ..\build

IF NOT EXIST .\assets mkdir .\assets

:: TODO(nick): link SDL libs
:: TODO(nick): create a SDL x86 / x64 build
::cl -MTd -Od -nologo -Zi  ..\code\main.cpp /I ..\code\include /link -incremental:no -opt:ref /LIBPATH:..\code\sdl2_x86\lib SDL2.lib /ENTRY:main

cl -MTd -Od -nologo -Zi ..\code\main.cpp /I ..\code\include /link /LIBPATH:..\code\sdl2_x86\lib kernel32.lib legacy_stdio_definitions.lib winmm.lib gdi32.lib user32.lib mincore.lib shell32.lib version.lib imm32.lib sdl2.lib sdl2main.lib sdl2_image.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrtd.lib

copy ..\code\sdl2_x86\lib\SDL2.dll .\

copy ..\code\sdl2_x86\lib\SDL2_Image.dll .\

copy ..\code\sdl2_x86\lib\libpng16-16.dll .\

copy ..\code\sdl2_x86\lib\zlib.dll .\

copy ..\assets\* .\assets\

popd

