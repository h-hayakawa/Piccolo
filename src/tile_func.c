#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include"tile_func.h"
#include"png_out.h"
#include"config.h"

//ビットマップバイト幅の算出マクロ
#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)
#endif//WIDTHBYTES

int32_t tile(char *filename, Bmp *bmp){
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
            SaveBitmapAsPngFile(fname, &tile);
        }
    }
    free(tile.map);
    return 1;
}


