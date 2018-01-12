all: core_app bmp2tile
core_app:
	gcc -o ../bin/core_app main.c io_func.c png_out.c bitmapio.c libpng.a -lz -lm -O3
bmp2tile:
	gcc -o ../bin/bmp2tile bmp2tile.c png_out.c bitmapio.c libpng.a -lz -lm -O3

clean:
	-rm ../bin/core_app
	-rm ../bin/bmp2tile
