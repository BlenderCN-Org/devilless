
Switches=(-g -O0 -Wall -Werror)

Files=( \
	/home/nathan/work/devilless/code/main_linux.cpp \
	/home/nathan/work/devilless/code/main.cpp \
	/home/nathan/work/devilless/code/renderer_opengl_x11.cpp \
	/home/nathan/work/devilless/code/renderer_opengl.cpp \
)

IncludePaths=( \
	-I ../code \
)

Libraries=( \
	-lX11 \
	-lGL \
	-pthread \
)

cd ..
g++ -o devilless ${Switches[@]} ${Files[@]} ${IncludePaths[@]} ${Libraries[@]}