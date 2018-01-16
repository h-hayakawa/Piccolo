#ifndef APP_H
#define APP_H

#include<stdint.h>
#include<stdio.h>
#include"bitmapio.h"

#define MAX_N_ARG 64
#define COMMAND_BUF_SIZE 4096

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

struct command_name_to_func{
  uint8_t *name;
  int32_t (*func)(App *app, int32_t argc, uint8_t **argv);
};

#endif
