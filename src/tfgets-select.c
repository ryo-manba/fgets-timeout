#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define TIMEOUT 5

char *tfgets(char *str, int size, FILE *stream)
{
  fd_set rfds;
  struct timeval tv;
  int retval;

  // FILE構造体からディスクリプタを取得する
  int fd = fileno(stream);

  // 監視対象のfdをセットする
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);

  // 監視する時間を設定する
  tv.tv_sec = TIMEOUT;
  tv.tv_usec = 0;

  // timeoutする -> 0
  // fdが読み込み可能になる -> 1 (読み込み可能になったfdの数)
  retval = select(1, &rfds, NULL, NULL, &tv);
  if (retval > 0)
    return fgets(str, size, stream);
  else
    return NULL;
}
