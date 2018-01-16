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

#define LOG_DIR "/var/www/Flute/bin/log"

void log_write(uint8_t *str){
  FILE *log;
  log = fopen(LOG_DIR, "a");
  fprintf(log, str);
  fclose(log);
}

void app_init(App* app){
  app->app_mode = APP_RUNNING;
  app->P_I = NULL;
  app->P_O = NULL;
  app->bmp.width = 0;
  app->bmp.height = 0;
  app->bmp.bit_count = 0;
  app->bmp.map = NULL;
}

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
int32_t app_output_tile_all(App *app, int32_t argc, uint8_t *argv[]){
  if (argc < 2){
    printf("%sに必要な引数が足りません．\n",argv[0]);
  }
  if (app->bmp.map == NULL){
    printf("%s:画像ファイルがロードされていません\n",argv[0]);
    return 0;
  }
  tile(argv[1], &app->bmp);
  if (app->P_O){
    fprintf(app->P_O, "done\n");
    fflush(app->P_O);
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
  uint8_t logbuf[4096];
  sprintf(logbuf, "timeout\n");
  log_write(logbuf);
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

int main(int argc, char *argv[]){
  App app;
  uint8_t command_buf[COMMAND_BUF_SIZE];
  uint8_t *arg[MAX_N_ARG];
  int32_t n_arg;
  int32_t i;
  uint8_t logbuf[4096];
  
  /* set callback function at alarm signal */
  void* oldSigFunc = signalx(SIGALRM, sigFunc);
  
  struct command_name_to_func command_array_main[] = {
    {"load_bmp", app_load_bmp},
    {"output_png", app_output_png},
    {"output_tile_all", app_output_tile_all},
    {"exit", app_exit},
  };
  int32_t n_curr_command = sizeof(command_array_main)/sizeof(command_array_main[0]);
  
  app_init(&app);
  
  if (argc > 1){
    app.P_I = fopen(argv[1], "r+");
    if (app.P_I == NULL){
      sprintf(logbuf, "fifo %s のオープンに失敗しました\n", argv[1]);
      log_write(logbuf);
      return 0;
    }
  }
  if (argc > 2){
    app.P_O = fopen(argv[2], "w+");
    if (app.P_O == NULL){
      sprintf(logbuf, "fifo %s のオープンに失敗しました\n", argv[2]);
      log_write(logbuf);
      return 0;
    }
  }

  
  sprintf(logbuf, "in%s out%s\n", argv[1], argv[2]);
  log_write(logbuf);
  
  sprintf(logbuf, "start\n");
  log_write(logbuf);
  
  while(app.app_mode == APP_RUNNING){
    /* set timeout value */
    alarm(TIMEOUT_SEC);
    n_arg = read_command(&app, command_buf, COMMAND_BUF_SIZE, arg, MAX_N_ARG);
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
  sprintf(logbuf, "end\n");
  log_write(logbuf);
  /* reset callback function */
  signalx(SIGALRM, oldSigFunc);
  app_exit(&app, 0, NULL);
  /* reset timeout value */
  alarm(0);
}
