#ifndef APP_H
#define APP_H

#include<stdint.h>
#include<stdio.h>

enum{
  APP_RUNNING = 0,
  APP_EXIT
};

typedef struct __App__{
  int32_t app_mode;
  uint8_t curr_file_path[256];
  Bmp bmp;
} App;


#endif
