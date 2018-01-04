#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"config.h"
#include"bitmapio.h"
#include"png.h"

int main(int argc, char *argv[]){
  Bmp bmp;
  clock_t t1, t2;
  double time;
  char *read_fname;
  char *write_fname;
  
  if (argc < 3){
    printf("usage: bmp2png input-file-name output-file-name\n");
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
  write_bmp(write_fname, &bmp);
  t2 = clock();
  time = (double)(t2 - t1)/CLOCKS_PER_SEC;
  printf("tile write time = %f sec\n", time);
  /////////////////////////////////////////////

  delete_bmp(&bmp);
}