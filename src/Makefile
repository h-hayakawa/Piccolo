all: bmp2tile bmp2png bmp2bmp24
bmp2tile: bmp2tile.c
	gcc -o ../bin/bmp2tile bmp2tile.c bitmapio.c libpng.a -lz -lm -O3
bmp2png: bmp2png.c
	gcc -o ../bin/bmp2png bmp2png.c bitmapio.c libpng.a -lz -lm -O3
bmp2bmp24: bmp2bmp24.c
	gcc -o ../bin/bmp2bmp24 bmp2bmp24.c bitmapio.c libpng.a -lz -lm -O3

clean:
	-rm ../bin/test
	-rm ../bin/bmp2png
	-rm ../bin/bmp2bmp24