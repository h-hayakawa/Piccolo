all: core_app
core_app:
	gcc -o ../bin/core_app main.c io_func.c png_out.c bitmapio.c tile_func.c libpng.a -lz -lm -O3

clean:
	-rm ../bin/core_app
