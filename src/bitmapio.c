#include"bitmapio.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)
#endif//WIDTHBYTES

void init_mono_bmp(Bmp *bmp){
  bmp->width = 0;
  bmp->height = 0;
  bmp->map = NULL;
}


void delete_bmp(Bmp *bmp){
  bmp->height = 0;
  bmp->width = 0;
  free(bmp->map);
  bmp->map = NULL;
}

int alloc_bmp(int width,int height,Bmp *bmp){
    int i;
    unsigned int dwWidthBytes, cbBits;
    int nDepth;
    
    bmp->width = width;
    bmp->height = height;
    
    nDepth = 24; //24bit BGR
    dwWidthBytes = WIDTHBYTES(bmp->width * nDepth);
    cbBits = dwWidthBytes * bmp->height;
    
    bmp->map = malloc(cbBits);
    if(bmp->map == NULL){
      return 0;
    }
    return 1;
}

int32_t get_file_size(uint8_t *filename){
  FILE *fp;
  long file_size;
  char *buffer;
  struct stat stbuf;
  int fd;
  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    /* エラー処理 */
  }
  fp = fdopen(fd, "rb");
  if (fp == NULL) {
    return -1;
  }
  if (fstat(fd, &stbuf) == -1) {
    return -1;
  }
  file_size = stbuf.st_size;
  return file_size;
}

/* ヘッダより大きいバイト数しかダメ(138以上) */
#define HEADER_BUF_SIZE 256


BMP_IO_ERR_CODE load_bmp_file(char* filename, Bmp *bmp){
  int32_t i,j,k;
  int32_t bk=-1,wh=-1;//色アクセス用変数
  uint32_t temp;
  uint32_t mask;
  int32_t dwWidthBytes, cbBits;
  int32_t nDepth;

  uint8_t header_buf[HEADER_BUF_SIZE];
  int32_t buf_size;
  int32_t buf_pt;
  int32_t file_size;
  int32_t remain_file_size;

  BmpHeader bf;
  BmpInfo bi;

  FILE *fp;
  fp = fopen(filename,"rb");

  if( fp == NULL ){
    return BMP_IO_ERROR_FAILD_TO_OPEN_FILE;
  }
  remain_file_size = file_size = get_file_size(filename);
  
  //ファイルヘッダ部読み込み(14 byte固定長)
  buf_size = 14;
  if (remain_file_size < buf_size){
    buf_size = remain_file_size;
  }
  if (!fread(header_buf, buf_size, 1, fp)){
    return BMP_IO_ERROR_INVALID_FILE_TYPE;
  }
  remain_file_size -= buf_size;
  buf_pt = 0;
  bf.FileType = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8);
  buf_pt += 2;
  bf.FileSize = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bf.Reserved1 = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8);
  buf_pt += 2;
  bf.Reserved2 = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8);
  buf_pt += 2;
  bf.OffBits = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  /*
  printf("head %x\n", bf.FileType);
  printf("size %d\n", bf.FileSize);
  printf("offbits %d\n", bf.OffBits);
  */
  //ビットマップはバイナリ先頭が"BM"(4d42)
  if (bf.FileType != 0x4D42){
    return BMP_IO_ERROR_INVALID_FILE_TYPE;
  }
  
  //情報ヘッダ読み込み
  buf_size = 40;
  if (remain_file_size < buf_size){
    buf_size = remain_file_size;
    return BMP_IO_ERROR_INVALID_FILE_TYPE;
  }
  if (!fread(header_buf, buf_size, 1, fp)){
    return BMP_IO_ERROR_INVALID_FILE_TYPE;
  }
  remain_file_size -= buf_size;
  buf_pt = 0;
  bi.HeaderSize = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.Width = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.Height = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.Planes = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8);
  buf_pt += 2;
  bi.BitCounts = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8);
  buf_pt += 2;
  bi.Compression = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.DataSize = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.XPelsPerMeter = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.YPelsPerMeter = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.ClrUsed = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
  bi.ClrImpotant = (header_buf[buf_pt]) + (header_buf[buf_pt + 1] << 8) + (header_buf[buf_pt + 2] << 16) + (header_buf[buf_pt + 3] << 24);
  buf_pt += 4;
/*
  printf("HeaderSize %d\n", bi.HeaderSize);
  printf("Width %d\n", bi.Width);
  printf("Height %d\n", bi.Height);
  printf("Planes %d\n", bi.Planes);
  printf("BitCounts %d\n", bi.BitCounts);
  printf("Compression %d\n", bi.Compression);
  printf("DataSize %d\n", bi.DataSize);
  printf("XPelsPerMeter %d\n", bi.XPelsPerMeter);
  printf("YPelsPerMeter %d\n", bi.YPelsPerMeter);
  printf("ClrUsed %d\n", bi.ClrUsed);
  printf("ClrImpotant %d\n", bi.ClrImpotant);
  */
  //情報ヘッダ読み込み
  buf_size = bi.HeaderSize - 40;
  if (buf_size){
    if (remain_file_size < buf_size){
      buf_size = remain_file_size;
      return BMP_IO_ERROR_INVALID_FILE_TYPE;
    }
    if (!fread(header_buf, buf_size, 1, fp)){
      return BMP_IO_ERROR_INVALID_FILE_TYPE;
    }
    remain_file_size -= buf_size;
    buf_pt = 0;
  }
  if (bi.BitCounts == 1 && bi.ClrUsed == 2){ //モノクロビットマップの場合
    uint8_t *databuf;
    uint32_t w_bytes = WIDTHBYTES(bi.Width * bi.BitCounts);
    /* カラーパレット読み込み */
    buf_size = 8;
    if (buf_size){
      if (remain_file_size < buf_size){
        buf_size = remain_file_size;
        return BMP_IO_ERROR_INVALID_FILE_TYPE;
      }
      if (!fread(header_buf, buf_size, 1, fp)){
        return BMP_IO_ERROR_INVALID_FILE_TYPE;
      }
      remain_file_size -= buf_size;
      buf_pt = 0;
    }
    //color[0]が白の場合
    if(header_buf[0]==255 && header_buf[1]==255 && header_buf[2]==255)
      wh=0;
    //color[0]が黒
    else if(header_buf[0]==0 && header_buf[1]==0 && header_buf[2]==0)
      bk=0;
    //color[1]が白の場合
    if(header_buf[4]==255 && header_buf[5]==255 && header_buf[6]==255)
      wh=1;
    //color[1]が黒
    else if(header_buf[4]==0 && header_buf[5]==0 && header_buf[6]==0)
      bk=1;
    if(wh+bk!=1){ /* 白黒じゃない */
      return BMP_IO_ERROR_INVALID_FILE_TYPE;
    }
    
    databuf = malloc(w_bytes);
    
    if(bi.Height > 0){
      bmp->height = bi.Height;
    } else {
      bmp->height = - bi.Height;
    }
    bmp->width = bi.Width;
    bmp->bit_count = 24;
    //メモリー領域確保
    if(alloc_bmp(bmp->width, bmp->height,bmp)==0){
      fclose(fp);
      return BMP_IO_ERROR_FAILD_TO_ALLOCATE_MEMORY;
    }
    
    temp=0;
    mask = ~(unsigned int)((int)0-bk);
    dwWidthBytes = WIDTHBYTES(bi.Width * bmp->bit_count);

    for(i = 0 ; i < bmp->height ;i ++ ){
      int h;
      if(bi.Height > 0){
        h = bi.Height - i - 1;
      } else {
        h = i;
      }
      if(!fread(databuf, w_bytes, 1, fp)){
        goto READ_END_MONO;
      }
      for(j = 0;j < (bmp->width >> 3) ;j+= 4){
        temp = (databuf[j] << 24) + (databuf[j + 1] << 16) + (databuf[j + 2] << 8) + (databuf[j + 3]);
        temp ^= mask;
        for(k = 0;k < 32 && (j * 8 + k) < bmp->width ;k++){
          int tmp = (temp>>(31-k))&1;
          bmp->map[dwWidthBytes * h + ((j << 3) + k)*3 + 0] = 255 - tmp * 255;
          bmp->map[dwWidthBytes * h + ((j << 3) + k)*3 + 1] = 255 - tmp * 255;
          bmp->map[dwWidthBytes * h + ((j << 3) + k)*3 + 2] = 255 - tmp * 255;
        }
      }
    }
    READ_END_MONO:;
    fclose(fp);
    free(databuf);
    return BMP_IO_SUCCESS;
  } else if (bi.BitCounts == 24){ // 24ビットビットマップの場合
    uint8_t *databuf;
    uint32_t w_bytes = WIDTHBYTES(bi.Width * bi.BitCounts);
    dwWidthBytes = WIDTHBYTES(bi.Width * bmp->bit_count);
    
    databuf = malloc(w_bytes);
    
    if(bi.Height > 0){
      bmp->height = bi.Height;
    } else {
      bmp->height = - bi.Height;
    }
    bmp->width = bi.Width;
    bmp->bit_count = 24;
    //メモリー領域確保
    if(alloc_bmp(bmp->width, bmp->height,bmp)==0){
      fclose(fp);
      return BMP_IO_ERROR_FAILD_TO_ALLOCATE_MEMORY;
    }
    
    dwWidthBytes = WIDTHBYTES(bi.Width * bmp->bit_count);
    
    for(i = 0 ; i < bmp->height ;i ++ ){
      int h;
      if(bi.Height > 0){
        h = bi.Height - i - 1;
      } else {
        h = i;
      }
      if(!fread(databuf, w_bytes, 1, fp)){
        goto READ_END_24BIT;
      }
      memcpy(bmp->map + (dwWidthBytes * h), databuf, w_bytes);
    }
    READ_END_24BIT:;
    fclose(fp);
    free(databuf);
    return BMP_IO_SUCCESS;
  } else {
    /* 許可していないファイルタイプ */
    return BMP_IO_ERROR_INVALID_FILE_TYPE;
  }
}
void write_bmp(char *filename, Bmp *bmp){
    FILE *fp;
    BmpHeader bf;
    BmpInfo bi;
    int32_t file_header_size = 14;
    int32_t info_header_size = 40;
    uint8_t header_buf[HEADER_BUF_SIZE];
    int32_t header_buf_ptr = 0;
    uint32_t w_bytes = WIDTHBYTES(bmp->width * bmp->bit_count);
    int32_t data_size = w_bytes * bmp->height;
    int32_t i;
    
    fp = fopen(filename, "wb");
    if (!fp) {
        return;
    }
    bf.FileType = 0x4D42;
    bf.FileSize = file_header_size + info_header_size + data_size;
    bf.Reserved1 = 0;
    bf.Reserved2 = 0;
    bf.OffBits = file_header_size + info_header_size; //ファイルヘッダ14byte + INFOヘッダ40byte
    
    bi.HeaderSize = info_header_size;
    bi.Width = bmp->width;
    bi.Height = bmp->height;
    bi.Planes = 1;
    bi.BitCounts = 24;
    bi.Compression = 0;
    bi.DataSize = data_size;
    bi.XPelsPerMeter = 0;
    bi.YPelsPerMeter = 0;
    bi.ClrUsed = 0;
    bi.ClrImpotant = 0; 
    
    header_buf[header_buf_ptr++] = (bf.FileType & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.FileType >> 8) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bf.FileType & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.FileType >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.FileType >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.FileType >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bf.Reserved1 & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.Reserved1 >> 8) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bf.Reserved2 & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.Reserved2 >> 8) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bf.OffBits & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.OffBits >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.OffBits >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bf.OffBits >> 24) & 0xFF);
    ///////////////
    header_buf[header_buf_ptr++] = (bi.HeaderSize & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.HeaderSize >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.HeaderSize >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.HeaderSize >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.Width & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Width >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Width >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Width >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.Height & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Height >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Height >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Height >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.Planes & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Planes >> 8) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.BitCounts & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.BitCounts >> 8) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.Compression & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Compression >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Compression >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.Compression >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.DataSize & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.DataSize >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.DataSize >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.DataSize >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.XPelsPerMeter & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.XPelsPerMeter >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.XPelsPerMeter >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.XPelsPerMeter >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.YPelsPerMeter & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.YPelsPerMeter >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.YPelsPerMeter >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.YPelsPerMeter >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.ClrUsed & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrUsed >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrUsed >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrUsed >> 24) & 0xFF);
    
    header_buf[header_buf_ptr++] = (bi.ClrImpotant & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrImpotant >> 8) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrImpotant >> 16) & 0xFF);
    header_buf[header_buf_ptr++] = ((bi.ClrImpotant >> 24) & 0xFF);

    fwrite (header_buf, header_buf_ptr, 1, fp);
    
    for(i = bmp->height - 1 ; i >= 0 ; i--){
        fwrite (bmp->map + i*w_bytes ,w_bytes , 1, fp);
    }

    fclose(fp);
}

