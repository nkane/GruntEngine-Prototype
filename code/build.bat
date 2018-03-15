:: Go up one directory from code
popd

IF NOT EXIST ..\build mkdir ..\build

pushd ..\build

IF NOT EXIST .\assets mkdir .\assets

cl -MTd -Od -nologo -Zi ..\code\grunt_main.cpp /I ..\code\include /link /LIBPATH:..\code\sdl2_x86\lib kernel32.lib legacy_stdio_definitions.lib winmm.lib gdi32.lib user32.lib mincore.lib shell32.lib version.lib imm32.lib sdl2.lib sdl2main.lib sdl2_image.lib SDL2_ttf.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrtd.lib

copy ..\code\sdl2_x86\lib\SDL2.dll .\ >NUL

copy ..\code\sdl2_x86\lib\SDL2_Image.dll .\ >NUL

copy ..\code\sdl2_x86\lib\libpng16-16.dll .\ >NUL

copy ..\code\sdl2_x86\lib\zlib1.dll .\ >NUL

copy ..\code\sdl2_x86\lib\libfreetype-6.dll .\ >NUL

copy ..\code\sdl2_x86\lib\SDL2_ttf.dll .\ >NUL

xcopy ..\assets .\assets /S /K /F /I /Y >NUL

xcopy ..\data .\data /S /K /F /I /Y >NUL

popd

