#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"config.h"
#include"bitmapio.h"
#include"png.h"

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

int tile(char *filename, Bmp *bmp){
    clock_t t1, t2;
    double time;
    int tile_x = (bmp->width + (TILE_W - 1)) / TILE_W;
    int tile_y = (bmp->height + (TILE_H - 1)) / TILE_H;
    int i,j;
    int ti, tj;
    int nDepth = 24; //24bit BGR
    int tileWidthBytes = WIDTHBYTES(TILE_W * nDepth);
    int dwWidthBytes = WIDTHBYTES(bmp->width * nDepth);
    unsigned int cbBits = tileWidthBytes * TILE_H;
    
    Bmp tile;
    tile.bit_count = 24;
    tile.map = malloc(cbBits);
    if (!tile.map){
        return 0;
    }
    
    char fname[1024];
    
    for (tj = 0 ; tj < tile_y ; tj ++){
        for (ti = 0 ; ti < tile_x ; ti ++){
            int ip, jp;
            sprintf(fname, "%s_y%d_x%d.png", filename, tj, ti);
            printf("%s\n", fname);
            jp = tj * TILE_H;
            ip = ti * TILE_W;
            
            tile.height = TILE_H;
            tile.width = TILE_W;
            tileWidthBytes = WIDTHBYTES(TILE_W * nDepth);
            
            if (ip + TILE_W > bmp->width){
                tile.width = bmp->width - ip;
                tileWidthBytes = WIDTHBYTES(tile.width * nDepth);
            }
            if (jp + TILE_H > bmp->height){
                tile.height = bmp->height - jp;
            }
            for (j = 0 ; j < TILE_H ; j++){
                if (j + jp > bmp->height){
                    goto END;
                }
                for (i = 0; i < TILE_W ; i++){
                    int cp = TILE_W;
                    if (ip + TILE_W > bmp->width){
                        cp = bmp->width - ip;
                    }
                    cp *= 3;
                    memcpy(tile.map + (tileWidthBytes * j + i*3), bmp->map + (dwWidthBytes * (j + jp) + (i + ip)*3), cp);
                }
            }
            END:;
            t1 = clock();
            SaveBitmapAsPngFile(fname, &tile);
            t2 = clock();
            time = (double)(t2 - t1)/CLOCKS_PER_SEC;
            printf("write time = %f sec\n", time);
        }
    }
    
    free(tile.map);
    return 1;
}

int main(int argc, char *argv[]){
  Bmp bmp;
  clock_t t1, t2;
  double time;
  char *read_fname;
  char *write_fname;
  
  if (argc < 3){
    printf("usage: bmp2tile input-file-name output-file-name\n");
    return 0;
  } 
  read_fname = argv[1];
  write_fname = argv[2];
  
  init_mono_bmp(&bmp);
  t1 = clock();
  load_bmp_file(read_fname, &bmp);
  t2 = clock();
  time = (double)(t2 - t1)/CLOCKS_PER_SEC;
  printf("load time = %f sec\n", time);
  //////////////////////////////////////
  t1 = clock();
  tile(write_fname, &bmp);
  t2 = clock();
  time = (double)(t2 - t1)/CLOCKS_PER_SEC;
  printf("tile write time = %f sec\n", time);
  /////////////////////////////////////////////

  delete_bmp(&bmp);
}
