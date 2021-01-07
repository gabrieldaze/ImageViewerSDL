all: main.cc
	clang++ main.cc -o img -lsdl2 -lsdl2_image
