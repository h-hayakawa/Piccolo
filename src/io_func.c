#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include"io_func.h"


/*! @brief 標準入力からコマンドを読み込む．エスケープ処理も行われる．
 *  標準入力からの入力をスペース(1つ以上)区切りでトークン分解して返す．
 *  クォート処理，エスケープ処理も行われる．""の内部のスペースは区切り文字とみなされない．
 *  ""の内部で改行を行った場合は改行を通常の入力とみなし，入力は継続される．
 *  \の直後の文字はその文字として処理される．改行も同様．
 *  分解されたトークン数がmax_n_argを超える場合は読み飛ばされる．
 *  改行（入力の終了）までに入力がbuf_sizeを超えた場合は捨てられる．
 *  @param [out] buf 読み込みバッファ
 *  @param [in] buf_size bufの大きさ
 *  @param [out] arg 分解されたトークンへの先頭ポインタ
 *  @param [in] max_n_arg argのサイズ（読み込むトークンの最大数）
 *  @return 読み込まれたトークン数を返す．シンタックスエラー時は0を返し，EOF時はEOFを返す．
 */
int32_t read_command(uint8_t *buf, int32_t buf_size, uint8_t *arg[], int32_t max_n_arg)
{
  int32_t i;
  int32_t quote_flag = 0;
  int32_t token_flag = 0;
  int32_t escape_flag = 0;
  int32_t continue_flag = 1;
  int32_t token_count = 0;
  int32_t len;

  i = 0;
  while (continue_flag){
    continue_flag = 0;
    if (buf_size - i < 2){
      printf("長すぎる入力の末尾が無視されました．\n");
      return token_count;
    }
    if (fgets(buf + i, (buf_size - i), stdin) == NULL){/* 単体のEOF */
      printf("EOFが入力されました．\n");
      return EOF;
    }
    if (feof(stdin)){
      printf("EOFが入力されました．\n");
      return EOF;
    }
    len = strlen(buf + i);
    if ((buf + i)[len - 1] != '\n' && len == (buf_size - i - 1)){/* バッファいっぱいまで読んだが改行が現れない */
      int32_t c;
      printf("長すぎる入力の末尾が無視されました．\n");
      /* 改行が出てくるまで読み捨てる．他の方法がわからない． */
      while ((c = getchar()) != EOF){
        if (c == '\n'){
          break;
        }
      }
      if (c == EOF){
        printf("EOFが入力されました．\n");
        return EOF;
      }
    }
    while (buf[i] != '\0'){
      /* エスケープ処理 */
      escape_flag = 0;
      if(buf[i] == '\\'){
        int32_t j;
        escape_flag = 1;
        if(buf[i + 1] == '\0'){
          /* バッファ終端直前にエスケープ記号があると起こりうる． */
          printf("エスケープ記号の後にNULL文字が現れました．\n");
          return 0;
        }
        j = i;
        do{/* 余裕があればもっとマシな方法にしたい */
          buf[j] = buf[j + 1];
          j++;
        }while(buf[j] != '\0');
      } 
      /* クォート処理 */
      if(buf[i] == '"' && escape_flag == 0){
        int32_t j;
        /* トークン途中で閉じたのに次の文字が空白又,改行，NULLじゃない */
        if(token_flag && buf[i + 1] != ' ' && buf[i + 1] != '\n' && buf[i + 1] != '\0'){
          printf("'\"'がトークンの途中で使用されました．'\\\"'とエスケープしてください．\n");
          return 0;
        }
        //if(buf[i + 1] == '"' && quote_flag == 0){
        //  printf("長さ0のトークンが与えられました．1文字以上のトークンを指定してください．\n");
        //  return 0;
        //}
        //クォートされたが有効文字無く閉じられた
        if (quote_flag && token_flag == 0){
          if(token_count == max_n_arg){
            printf("多すぎるトークンが無視されました\n");
            return token_count;
          }
          buf[i] = '\0';
          arg[token_count++] = buf + i;
          quote_flag = 0;
          continue;
        }
        j = i;
        do{
          buf[j] = buf[j + 1];
          j++;
        }while(buf[j] != '\0');
        quote_flag^=1;
        continue;/* 次の文字を読む */
      }
      /* 空白文字じゃない　又はエスケープ中，クォート中 */
      if(((buf[i] != ' ' && buf[i] != '\n') || escape_flag || quote_flag) && token_flag == 0){
        token_flag = 1;
        if(token_count == max_n_arg){
          printf("多すぎるトークンが無視されました\n");
          return token_count;
        }
        arg[token_count++] = buf + i;
      }
      /* 区切り文字として有効な空白->NULL文字に書き換える */
      if(buf[i]==' ' && escape_flag == 0 && quote_flag == 0){
        buf[i] = '\0';
        token_flag = 0;
      }

      /* 改行の扱い */
      if(buf[i] == '\n'){
        /* エスケープでもクォートでもない改行は末尾なのでNULL文字に． */
        if(escape_flag == 0 && quote_flag == 0){
          buf[i] = '\0';
          break;
        }else{
          continue_flag = 1;
        }
      }
      i++;
    }
  }
  return token_count;
}
