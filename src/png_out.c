#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"config.h"
#include"bitmapio.h"
#include"png.h"
#include"io_func.h"
#include"png_out.h"

//ビットマップバイト幅の算出マクロ
#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)
#endif//WIDTHBYTES

int SaveBitmapAsPngFile(char* filename, Bmp *bmp)
{
    png_structp png;
    png_infop info;
    png_color_8 sBIT;
    png_bytep *lines;
    FILE *outf;

    unsigned int dwWidthBytes, cbBits;
    unsigned char * pbBits;
    int x, y;
    int nDepth;
    
    nDepth = 24; //24bit BGR
    dwWidthBytes = WIDTHBYTES(bmp->width * nDepth);

    outf = fopen(filename, "wb");
    if (!outf)
    {
        return 0;
    }

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        fclose(outf);
        return 0;
    }

    info = png_create_info_struct(png);
    if (info == NULL)
    {
        png_destroy_write_struct(&png, NULL);
        fclose(outf);
        return 0;
    }

    lines = NULL;
    if (setjmp(png_jmpbuf(png)))
    {
        fclose(outf);
        return 0;
    }

    png_init_io(png, outf);
    png_set_IHDR(png, info, bmp->width, bmp->height, 8, 
        (nDepth == 32 ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB),
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_BASE);

    sBIT.red = 8;
    sBIT.green = 8;
    sBIT.blue = 8;
    sBIT.alpha = 0;
    png_set_sBIT(png, info, &sBIT);

    png_write_info(png, info);
    png_set_bgr(png);

    lines = (png_bytep *)malloc(sizeof(png_bytep *) * bmp->height);
    for (y = 0 ; y < bmp->height ; y++){
        lines[y] = (bmp->map + dwWidthBytes * y);
    }

    png_set_compression_level(png, PNG_COMPRESSION_LEVEL); 
    png_write_image(png, lines);
    png_write_end(png, info);
    png_destroy_write_struct(&png, &info);
    free(lines);
    fclose(outf);
    return 1;
}
