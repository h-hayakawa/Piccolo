#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<signal.h>
#include"config.h"
#include"bitmapio.h"
#include"png.h"
#include"io_func.h"
#include"png_out.h"
#include"app.h"

/*
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
*/

void app_init(App* app){
  app->curr_file_path[0] = '\0';
  app->app_mode = APP_RUNNING;
  app->bmp.width = 0;
  app->bmp.height = 0;
  app->bmp.bit_count = 0;
  app->bmp.map = NULL;
}

struct command_name_to_func{
  uint8_t *name;
  int32_t (*func)(App *app, int32_t argc, uint8_t **argv);
};

int32_t exec_command(App* app, int32_t argc, uint8_t *argv[], struct command_name_to_func command_array[], int32_t n_func){
  int32_t i;
  /* コマンドが多いわけではないのでコマンド探索ツリーとかは作らない予定 */
  for (i = 0 ; i < n_func ; i++){
    if (strcmp(argv[0], command_array[i].name) == 0){
      return command_array[i].func(app, argc, argv);
    }
  }
  printf("%sというコマンドが見つかりません．\n", argv[0]);
  return -1;
}

static
int32_t app_load_bmp(App *app, int32_t argc, uint8_t *argv[])
{
  int32_t i;
  if (argc < 2){
    printf("%sに必要な引数が足りません．\n",argv[0]);
  }
  if (argc > 2){
    printf("%sに指定された余分な引数が無視されました．\n",argv[0]);
  }
  if (app->bmp.map){
    delete_bmp(&app->bmp);
  }
  if (load_bmp_file(argv[1], &app->bmp) != BMP_IO_SUCCESS){
    printf("%s:指定されたファイルのロードに失敗しました\n",argv[0]);
    return 0;
  }
  return 0;
}

static
int32_t app_output_png(App *app, int32_t argc, uint8_t *argv[])
{
  int32_t i;
  if (argc < 2){
    printf("%sに必要な引数が足りません．\n",argv[0]);
  }
  if (argc > 2){
    printf("%sに指定された余分な引数が無視されました．\n",argv[0]);
  }
  if (app->bmp.map == NULL){
    printf("%s:画像ファイルがロードされていません\n",argv[0]);
    return 0;
  }
  SaveBitmapAsPngFile(argv[1], &app->bmp);
  return 0;
}

static
int32_t app_exit(App *app, int32_t argc, uint8_t *argv[])
{
  if (argc > 1){
    printf("%sに指定された引数が無視されました．\n",argv[0]);
  }
  if (app->bmp.map){
    free(app->bmp.map);
    app->bmp.map = NULL;
  }
  app->app_mode = APP_EXIT;
  return 0;
}

/**********************************************/
/* function called by signal interupt         */
/**********************************************/
void sigFunc(int signo){
  return;
}

/**********************************************************/
/* signal function WITHOUT SA_RESTART                     */
/**********************************************************/
void *signalx(int signo, void* func)
{
  struct sigaction act, oldact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  if (sigaction(signo, &act, &oldact) < 0) return(SIG_ERR);
	
  return(oldact.sa_handler);
}


#define MAX_N_ARG 64
#define COMMAND_BUF_SIZE 4096
#define TIMEOUT_SEC 60

int main(){
  App app;
  uint8_t command_buf[COMMAND_BUF_SIZE];
  uint8_t *arg[MAX_N_ARG];
  int32_t n_arg;
  int32_t i;
  
  /* set callback function at alarm signal */
  void* oldSigFunc = signalx(SIGALRM, sigFunc);
  
  struct command_name_to_func command_array_main[] = {
    {"load_bmp", app_load_bmp},
    {"output_png", app_output_png},
    {"exit", app_exit},
  };
  int32_t n_curr_command = sizeof(command_array_main)/sizeof(command_array_main[0]);
  
  app_init(&app);
  while(app.app_mode == APP_RUNNING){
    /* set timeout value */
    alarm(TIMEOUT_SEC);
    n_arg = read_command(command_buf, COMMAND_BUF_SIZE, arg, MAX_N_ARG);
    alarm(0);
    if (n_arg > 0){
      int32_t r;
      clock_t t1, t2;
      double time;
      t1 = clock();
      r = exec_command(&app, n_arg, arg, command_array_main, n_curr_command);
      t2 = clock();
      time = (double)(t2 - t1)/CLOCKS_PER_SEC;
      if (r != -1){
        printf("exec %s time = %f sec\n", arg[0] , time);
      }
    }
    if (n_arg == EOF){
      break;
    }
  }
  /* reset callback function */
  signalx(SIGALRM, oldSigFunc);
  app_exit(&app, 0, NULL);
  /* reset timeout value */
  alarm(0);
}
