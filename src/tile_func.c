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

int32_t tile(Bmp *bmp, char *filename, int32_t y, int32_t x, int32_t m){
  uint8_t fname[1024];
  int32_t nDepth = 24; //24bit BGR
  
  sprintf(fname, "%s_%d_%d_%d.png", filename, y, x, m);
  int32_t i,j;
  int32_t dwWidthBytes = WIDTHBYTES(bmp->width * nDepth);
  int32_t tileWidthBytes = WIDTHBYTES(TILE_W * nDepth);
  uint32_t cbBits = tileWidthBytes * TILE_H;

  /* 有効範囲外の倍率指定 */
  if (sizeof(MAG_TAB)/sizeof(MAG_TAB[0]) <= m){
    return 0;
  }

  int32_t num = MAG_TAB[m].num;
  int32_t den = MAG_TAB[m].den;
  
  int32_t b_width = bmp->width * num / den;
  int32_t b_height = bmp->height * num / den;

  Bmp tile_tmp;
  tile_tmp.bit_count = 24;

  int32_t ip, jp;
  jp = y * TILE_H * den / num;
  ip = x * TILE_W * den / num;
  int32_t tip, tjp;
  tjp = y * TILE_H;
  tip = x * TILE_W;
  printf("%s\n", fname);
  printf ("%d %d\n", b_width , b_height);
  printf ("%d %d\n", jp , ip);
  
  tile_tmp.height = TILE_H;
  tile_tmp.width = TILE_W;
  tileWidthBytes = WIDTHBYTES(TILE_W * nDepth);
  
  if (tip + TILE_W > b_width){
    tile_tmp.width = b_width - tip;
    tileWidthBytes = WIDTHBYTES(tile_tmp.width * nDepth);
  }
  if (tjp + TILE_H > b_height){
    tile_tmp.height = b_height - tjp;
  }
  printf ("%d %d\n", tile_tmp.width , tile_tmp.height);
  if (tile_tmp.width < 1 || tile_tmp.height < 1){
    return 0;
  }
  //printf("%d %d\n", tile_tmp.width, tile_tmp.height);
  tile_tmp.map = malloc(cbBits);
  if (!tile_tmp.map){
    return 0;
  }
  /* 拡大出力 */
  if (num > den){
    for (j = 0 ; j < tile_tmp.height ; j++){
      int32_t by, bx;
      by = jp + j * den / num;
      if (by > bmp->height){
        break;
      }
      for (i = 0 ; i < tile_tmp.width ; i++){
        bx = ip + i * den / num;
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 0] = bmp->map[(dwWidthBytes * by) + bx * 3 + 0];
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 1] = bmp->map[(dwWidthBytes * by) + bx * 3 + 1];
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 2] = bmp->map[(dwWidthBytes * by) + bx * 3 + 2];
      }
    }
    SaveBitmapAsPngFile(fname, &tile_tmp);
  }
  
  if (num == den){
    /* 等倍出力 */
    for (j = 0 ; j < tile_tmp.height ; j++){
      if (j + jp > bmp->height){
        break;
      }
      int cp = TILE_W;
      if (ip + TILE_W > bmp->width){
          cp = bmp->width - ip;
      }
      cp *= 3;
      memcpy(tile_tmp.map + (tileWidthBytes * j), bmp->map + (dwWidthBytes * (j + jp) + (ip)*3), cp);
    }
    SaveBitmapAsPngFile(fname, &tile_tmp);
  }
  /* 縮小 */
  if (num < den){
    for (j = 0 ; j < tile_tmp.height ; j++){
      int32_t by, bx;
      int32_t byn, bxn;
      by = jp + j * den / num;
      if (by > bmp->height){
        break;
      }
      for (i = 0 ; i < tile_tmp.width ; i++){
        int32_t xx, yy;
        int32_t r, g, b;
        int32_t pc;
        bx = ip + i * den / num;
        
        byn = jp + (j+1) * den / num;
        bxn = ip + (i+1) * den / num;
        pc;
        r = 0; g = 0; b = 0;
        pc = 0;
        for (yy = by ; yy < byn ; yy++){
          if (yy > bmp->height){
            break;
          }
          for (xx = bx ; xx < bxn ; xx++){
            if (xx > bmp->width){
              break;
            }
            b += bmp->map[(dwWidthBytes * yy) + xx * 3 + 0];
            g += bmp->map[(dwWidthBytes * yy) + xx * 3 + 1];
            r += bmp->map[(dwWidthBytes * yy) + xx * 3 + 2];
            pc++;
          }
        }
        if (pc){
          b /= pc;
          g /= pc;
          r /= pc;
          if (b > 255){
            b = 255;
          }
          if (g > 255){
            g = 255;
          }
          if (r > 255){
            r = 255;
          }
        }
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 0] = b;
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 1] = g;
        tile_tmp.map[(tileWidthBytes * j) + i * 3 + 2] = r;
      }
    }
    SaveBitmapAsPngFile(fname, &tile_tmp);
  }
  free(tile_tmp.map);
  return 1;
}




