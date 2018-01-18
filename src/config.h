#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TILE_W 256
#define TILE_H 256

/* 操作をしなかった場合のタイムアウト設定（秒） */
#define TIMEOUT_SEC 10


#define LOG_DIR "/var/www/Flute/bin/log"

/* 0-9  6がデフォルト値っぽい */
#define PNG_COMPRESSION_LEVEL 9

typedef struct __mag__ {
  int32_t num; //分子
  int32_t den; //分子
} mag;

/* 表示倍率設定テーブル、{1,1}が等倍なので、mag_tab[3]が等倍 */
static
mag MAG_TAB[]={
  {8,1},//8倍
  {4,1},//4倍
  {2,1},//2倍
  {1,1},//等倍
  {1,2},//1/2倍
  {1,4},//1/4倍
  {1,8},//1/8倍
  {1,12},
  {1,16},//1/16倍
  {1,20},
  {1,24},
  {1,28},
  {1,32},//1/32倍
  {1,36},
  {1,40},
  {1,44},
  {1,48},
  {1,52},
  {1,56},
  {1,60},
  {1,64},//1/64倍
};

#endif
