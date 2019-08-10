
Switches=(-g -O0 -Wall -Werror)

Files=( \
	/home/nathan/work/devilless/code/main_linux.cpp \
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