#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define TIMEOUT 5

// スレッドの処理に与えられる引数が1つのみなので構造体にまとめる
typedef struct
{
  char *str;
  int size;
  FILE *stream;
  pthread_t sleep_tid;
} fgets_args_t;

// fgetsの戻り値として使用する
static char *retval;

static void *fgets_thread(void *vargp)
{
  // pthread_self：呼び出したスレッドのIDを取得する
  // pthread_detach：実行中のスレッドをデタッチ状態にする
  pthread_detach(pthread_self());
  fgets_args_t *argp = (fgets_args_t *)vargp;

  // fgetsを実行する
  retval = fgets(argp->str, argp->size, argp->stream);

  // スリープを実行しているスレッドにキャンセル要求を送る
  pthread_cancel(argp->sleep_tid);
  return NULL;
}

static void *sleep_thread(void *vargp)
{
  sleep(TIMEOUT);
  return NULL;
}

char *tfgets(char *str, int size, FILE *stream)
{
  pthread_t fgets_tid;
  pthread_t sleep_tid;

  // fgetsのスレッドの引数を作成する
  fgets_args_t args = {.str = str, .size = size, .stream = stream};
  retval = NULL;

  // sleepを実行するスレッドを作成する
  pthread_create(&sleep_tid, NULL, sleep_thread, NULL);

  // sleepスレッドのIDを引数に追加する
  args.sleep_tid = sleep_tid;

  // fgetsを実行するスレッドを作成する
  pthread_create(&fgets_tid, NULL, fgets_thread, &args);

  // sleepスレッドの終了を待つ
  pthread_join(sleep_tid, NULL);
  return retval;
}
