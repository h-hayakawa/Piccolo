#ifndef __BITMAP_IO_H__
#define __BITMAP_IO_H__

#include <stdint.h>

#define BMP_IO_ERR_CODE int32_t

enum{
BMP_IO_SUCCESS = 0,
BMP_IO_ERROR_FAILD_TO_OPEN_FILE,
BMP_IO_ERROR_INVALID_FILE_TYPE,
BMP_IO_ERROR_FAILD_TO_ALLOCATE_MEMORY
};


typedef struct __Bmp__{
  int32_t width;             /* x方向のピクセル数 */
  int32_t height;             /* y方向のピクセル数 */
  int32_t bit_count; /* 1ピクセルビット数 */
  uint8_t *map;  /* データ */
} Bmp;

typedef struct __BmpHeader__{
  uint16_t FileType;//ファイルタイプ
  uint32_t FileSize;//ファイルサイズ
  uint16_t Reserved1;//予約領域1 内容はアプリ固有
  uint16_t Reserved2;//予約領域2 内容はアプリ固有
  uint32_t OffBits;//ビットマップデータ領域までのバイト数?(0でもよい)62
}BmpHeader;

//INFOタイプ情報ヘッダ
typedef struct __BmpInfo__{
  uint32_t HeaderSize;//ヘッダサイズ
  int32_t Width;
  int32_t Height;
  uint16_t Planes;//チャンネル数 常に1
  uint16_t BitCounts;//ピクセル毎のビット数
  uint32_t Compression;//圧縮形式 0固定(非圧縮)
  uint32_t DataSize;//画像データのサイズ
  uint32_t XPelsPerMeter;
  uint32_t YPelsPerMeter;
  uint32_t ClrUsed;//カラーインデックス数
  uint32_t ClrImpotant;//重要インデックス数
}BmpInfo;

//モノクロビットマップのファイルを開く
BMP_IO_ERR_CODE load_mono_bmp_file(char *filename, Bmp *bmp);
BMP_IO_ERR_CODE load_bmp_file(char *filename, Bmp *bmp);

void init_mono_bmp(Bmp *bmp);
void delete_bmp(Bmp *bmp);
int32_t alloc_bmp(int32_t width,int32_t height,Bmp *bmp);
void write_bmp(char *filename, Bmp *bmp);

#endif
