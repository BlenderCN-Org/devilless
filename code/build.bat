@echo off

set Switches= -MTd -nologo -GR -EHa -Od -Oi -FC -Z7 -GS -Gs9999999 -WX -W4 -wd4505 -wd4201 -wd4100 -wd4189 -wd4996

set Files= ^
	w:\devilless\code\asset_manager.cpp ^
	w:\devilless\code\main.cpp ^
	w:\devilless\code\main_win.cpp ^
	w:\devilless\code\renderer_opengl_win.cpp

set IncludePaths= ^
	-I ../code

set Libraries= ^
	OpenGL32.lib ^
	User32.lib ^
	Gdi32.lib

pushd ..
cl -Fe: devilless.exe %Switches% %Files% %IncludePaths% %Libraries%
popd