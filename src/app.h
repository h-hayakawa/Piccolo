#ifndef APP_H
#define APP_H

#include<stdint.h>
#include<stdio.h>
#include"bitmapio.h"

enum{
  APP_RUNNING = 0,
  APP_EXIT
};

typedef struct __App__{
  int32_t app_mode;
  FILE *P_I;
  FILE *P_O;
  Bmp bmp;
} App;

#endif
